from sequence import Sequence
from typedefs import Track, NoteType, NoteEvent

class SequenceOptimizer:

    @staticmethod
    def _split_track_by_polyphony(track, final_poly):

        # Create voice tracks
        voices = [Track(
            name=f"{track.name}_v{i}",
            instrument=track.instrument,
            volume_source=track.volume_source,
            pan_source=track.pan_source,
            fine_pitch_source=track.fine_pitch_source,
            effect=track.effect,
            gm_program=track.gm_program
        ) for i in range(final_poly)]

        # Assign notes (greedy)
        note_events = []
        
        for note in track.notes:
            if note.type == NoteType.NOTE:
                note_events.append((note.ticks, note.ticks + note.duration_ticks, note))
        
        note_events.sort(key=lambda x: x[0])

        voice_free_at = [0] * final_poly
        
        for start, end, note in note_events:

            assigned = -1
            
            for v in range(final_poly):
                if voice_free_at[v] <= start:
                    assigned = v
                    break
            
            if assigned != -1:
                voices[assigned].notes.append(note)
                voice_free_at[assigned] = end

        # Handle instrument families (piano/strings splitting)
        is_piano = track.instrument in [9, 10, 25]
        is_strings = track.instrument in [47, 14, 16]

        for v_track in voices:

            if not v_track.notes: continue
            
            if is_piano or is_strings:
                notes = [n.note for n in v_track.notes]
                # TODO: might need to improve this heuristic
                avg = sum(notes) / len(notes)
                
                if is_piano:
                    
                    # Assign piano patches based on average pitch (notes are already transposed)
                    # Patch 9 (low piano): no lower limit --> F#2 (42)
                    # Patch 25 (mid piano): G2 (43) --> F#3 (54)
                    # Patch 10 (high piano): G3 (55) --> G5 (79)

                    if avg <= 42: v_track.instrument = 9 # Low piano
                    elif avg <= 54: v_track.instrument = 25 # Mid piano
                    else: v_track.instrument = 10 # High piano

                elif is_strings:
                    
                    # Assign string patches based on average pitch (notes are already transposed)
                    # Patch 47 (low strings): optimal at C#2 (25) - cello range
                    # Patch 14 (mid strings): optimal at A3 (57) - viola range
                    # Patch 16 (high strings): optimal at F4 (65) - violin range
                    
                    if avg <= 45: v_track.instrument = 47 # Low strings
                    elif avg <= 60: v_track.instrument = 14 # Mid strings
                    else: v_track.instrument = 16 # High strings

            # Re-add Rests
            v_track.notes.sort(key=lambda n: n.ticks)

            # Add rest events between notes and after notes that don't extend to next event
            filled_notes = []
            last_time = 0

            for n in v_track.notes:
                if n.ticks > last_time + 2:
                    filled_notes.append(NoteEvent(NoteType.REST, last_time))
                filled_notes.append(n)
                last_time = n.ticks + n.duration_ticks
            v_track.notes = filled_notes
            
        return [t for t in voices if t.notes]

    @staticmethod
    def split_polyphony(seq: Sequence, max_voices=None):
        """Splits polyphonic tracks into multiple monophonic tracks."""

        new_tracks = []

        for track_idx, track in enumerate(seq.tracks):

            if not track.notes:
                new_tracks.append(track)
                continue

            # 1. Calculate max polyphony
            # 'Sweepline' algorithm: +1 for note start, -1 for note end.
            # Store only (time, direction) to avoid unpacking errors.
            events = []
            for note in track.notes:
                if note.type == NoteType.NOTE:
                    events.append((note.ticks, 1))
                    events.append((note.ticks + note.duration_ticks, -1))
            
            # Sort by time (and then by direction if times are equal)
            events.sort()

            max_poly = 0
            curr_poly = 0
            for _, change in events:
                curr_poly += change
                max_poly = max(max_poly, curr_poly)
            
            # 2. Determine Final Voice Count
            # Limit polyphony if max voices specified
            final_poly = max_voices if max_voices and max_poly > max_voices else max_poly

            if final_poly <= 1:
                new_tracks.append(track)
            else:
                split_results = SequenceOptimizer._split_track_by_polyphony(track, final_poly)
                new_tracks.extend(split_results) 

        seq.tracks = new_tracks

    @staticmethod
    def compress_tracks(seq: Sequence):
        """Multiplexes instruments to reduce track count."""
        if len(seq.tracks) <= 1: return

        # Build timeline for each track showing active periods
        timelines = []

        for track in seq.tracks:
            tl = []
            for n in track.notes:
                if n.type == NoteType.NOTE:
                    # Use actual note duration, not time to next event
                    # (next event could be a rest, which would make the timeline wrong)
                    tl.append((n.ticks, n.ticks + n.duration_ticks))
            timelines.append(tl)

        # Try to merge tracks with non-overlapping or minimally overlapping notes
        # Sort by density (most notes first)
        track_indices = sorted(range(len(seq.tracks)), key=lambda i: len(seq.tracks[i].notes), reverse=True)
        
        merged_set = set() # Tracks that have been merged into others
        merge_map = {} # Maps source track -> (target track, merge points)

        for i, target_idx in enumerate(track_indices):
            if target_idx in merged_set: continue
            
            target_tl = timelines[target_idx]
            group_tls = [target_tl] # Track timelines already in this merge group

            for source_idx in track_indices[i+1:]:

                if source_idx in merged_set: continue
                
                source_tl = timelines[source_idx]
                
                # Check overlap
                can_merge = True

                for existing in group_tls:
                    if not SequenceOptimizer._can_multiplex(existing, source_tl):
                        can_merge = False
                        break
                
                if can_merge:
                    
                    if target_idx not in merge_map: merge_map[target_idx] = []

                    merge_map[target_idx].append(source_idx)
                    merged_set.add(source_idx)
                    group_tls.append(source_tl)

        new_tracks = []

        for i, track in enumerate(seq.tracks):

            # Already merged track 
            if i in merged_set: continue
            
            if i in merge_map:
                sources = [seq.tracks[s] for s in merge_map[i]]
                merged = SequenceOptimizer._merge(track, sources)
                new_tracks.append(merged)
            else:
                new_tracks.append(track)
        
        seq.tracks = new_tracks

    @staticmethod
    def _can_multiplex(tl1, tl2):
        for s1, e1 in tl1:
            for s2, e2 in tl2:
                if max(s1, s2) < min(e1, e2): return False # Any overlap
        return True
    
    @staticmethod
    def _merge(target, sources):
        """
        Merge multiple tracks into one with instrument switching.
        Returns a new Track with multiplexed notes.
        """
        all_tracks = [target] + sources
        raw_events = []
        
        # 1. Collect ONLY NOTES (ignore existing rests from source tracks to prevent duplicates)
        for t_idx, trk in enumerate(all_tracks):
            for note in trk.notes:
                if note.type == NoteType.NOTE:
                    raw_events.append((note.ticks, trk.instrument, trk.effect, note, t_idx))
        
        # Sort all notes by time
        raw_events.sort(key=lambda x: x[0])
        
        final_notes = []
        final_timeline = []
        last_end_time = 0
        
        # Default state tracking for timeline
        last_inst = target.instrument
        last_effect = target.effect

        # 2. Rebuild track with fresh Rests filling the gaps
        for evt in raw_events:
            
            # event structure: (ticks, instr, effect, note_obj, source_idx)
            start = evt[0]
            note_obj = evt[3]
            
            # If there is a significant gap, insert a fresh rest
            if start > last_end_time + 2:
                # Create rest
                rest_note = NoteEvent(NoteType.REST, last_end_time)
                final_notes.append(rest_note)
                
                # Add rest to timeline (needed to keep indices aligned in converter.py)
                # Preserve the last known instrument/effect state
                final_timeline.append((last_end_time, last_inst, last_effect, rest_note, -1))
            
            # Add the actual Note
            final_notes.append(note_obj)
            final_timeline.append(evt)
            
            last_end_time = start + note_obj.duration_ticks
            last_inst = evt[1]
            last_effect = evt[2]

        new_track = Track(
            name=f"Merged_{target.name}",
            instrument=target.instrument,
            notes=final_notes,
            volume_source=target.volume_source,
            pan_source=target.pan_source,
            fine_pitch_source=target.fine_pitch_source,
            effect=target.effect,
            instrument_timeline=final_timeline
        )

        return new_track