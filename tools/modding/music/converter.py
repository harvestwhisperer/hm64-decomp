import struct
import mido

from collections import Counter

from constants import OpCode, NOTE_REST
from config import get_adsr_for_instrument, is_multi_patch_instrument, get_best_instrument_for_note, get_game_instrument_from_gm, get_game_drum_instrument, ADSR_TABLE
from optimization import SequenceOptimizer
from controller_source import ControllerSource
from sequence import Sequence
from typedefs import ControllerEvent, ControllerType, NoteEvent, NoteType, Track

class SeqConverter:
    def __init__(self, use_gm_mapping=False, quantize_grid=None):
        self.seq = Sequence()
        self.use_gm_mapping = use_gm_mapping
        self.quantize_grid = quantize_grid

    def _encode_varlen(self, value: int) -> bytes:
        if value < 0x80: return bytes([value])
        return bytes([0x80 | ((value >> 8) & 0x7F), value & 0xFF])

    def _get_source_index(self, cur_sources, stype):
        
        for i, s in enumerate(cur_sources):
            if s.type == stype: return i
        
        # Create new source
        cur_sources.append(ControllerSource(type=stype))
        
        return len(cur_sources) - 1
    
    def _is_mostly_constant(self, values, threshold=0.7):
        """Check if >70% of values are the same"""

        if not values:
            return False
        
        counts = Counter(values)
        most_common_count = counts.most_common(1)[0][1]
        
        return most_common_count / len(values) >= threshold

    def _preprocess_quantize(self, mid, grid_size=10):
        """Quantize MIDI event times to grid boundaries (in-place)"""
        print(f"Quantizing MIDI to {grid_size}-tick grid...")

        total_adjustments = 0

        for track in mid.tracks:
            abs_time = 0
            quantized_events = []
            last_quantized_time = 0

            for msg in track:
                abs_time += msg.time
                quantized_abs_time = round(abs_time / grid_size) * grid_size

                if abs(quantized_abs_time - abs_time) > 0:
                    total_adjustments += 1

                delta = quantized_abs_time - last_quantized_time
                last_quantized_time = quantized_abs_time

                new_msg = msg.copy(time=delta)
                quantized_events.append(new_msg)

            track.clear()
            track.extend(quantized_events)

        print(f"  Adjusted {total_adjustments} events")

    def _preprocess_remove_unused_pcs(self, mid):
        """
        Remove unused program_change messages (in-place).

        This function performs two types of cleanup:
        1. Per-track: Remove program changes on channels that a track doesn't use
        2. Per-channel: Keep only the last program change before the first note on each channel

        This prevents issues where notation software includes program changes for
        multiple instrument variations (e.g., pizzicato, ensemble).
        """
        print("Removing unused program changes...")

        removed_count = 0

        # Process each track independently
        for idx, track in enumerate(mid.tracks):

            # Find which channels this specific track uses for notes
            track_channels = set()
            abs_time = 0
            first_note_time_per_channel = {}  # channel -> first note time

            for msg in track:
                abs_time += msg.time
                if msg.type == 'note_on' and msg.velocity > 0:
                    track_channels.add(msg.channel)
                    if msg.channel not in first_note_time_per_channel:
                        first_note_time_per_channel[msg.channel] = abs_time

            if not track_channels:
                # No notes in this track, skip
                continue

            # Build new message list, cleaning up program changes
            new_msgs = []
            abs_time = 0
            last_pc_per_channel = {}  # channel -> (time, msg_index, msg)

            # First pass: identify which program changes to keep
            for msg_idx, msg in enumerate(track):
                abs_time += msg.time

                if msg.type == 'program_change':
                    if msg.channel in track_channels:
                        # This channel is used by the track
                        first_note_time = first_note_time_per_channel[msg.channel]

                        if abs_time <= first_note_time:
                            # Program change before first note - track it
                            last_pc_per_channel[msg.channel] = (abs_time, msg_idx, msg)

            # Second pass: build cleaned message list
            abs_time = 0

            for msg_idx, msg in enumerate(track):
            
                abs_time += msg.time

                if msg.type == 'program_change':
                    # Check if this program change should be kept
                    if msg.channel not in track_channels:
                        # Channel not used by this track - remove
                        removed_count += 1
                        continue
                    elif msg.channel in last_pc_per_channel:
                        # Keep only the last program change before first note
                        last_pc_info = last_pc_per_channel[msg.channel]
                        if msg_idx != last_pc_info[1]:
                            # Not the last one - remove
                            removed_count += 1
                            continue
                    # else: program change after first note - keep it

                new_msgs.append(msg)

            track.clear()
            track.extend(new_msgs)

        if removed_count > 0:
            print(f"  Removed {removed_count} unused program changes")

    def _adsr_index_for_instrument(self, game_inst: int) -> int:
        """
        Set ADSRs based on generic hardcoded values from sample songs
        """
        # ADSR 0: Melodic/sustained instruments
        # Includes: accordion, bongos2, flute, strings (mid/high), clave, fiddle,
        # brass section, hi-hat closed, organ 2, cello, electric bass, saxophone
        adsr_0_instruments = {1, 5, 11, 14, 16, 28, 32, 38, 40, 41, 46, 57, 58}

        # ADSR 1: Percussive/short-attack
        # Includes: kick drum, bongos, tambourine, cowbell, unmapped instruments
        # Note: Instrument 5 (Bongos 2) appears in both ADSR 0 (village) and ADSR 1 (racing-festival)
        # Using ADSR 0 as primary
        adsr_1_instruments = {3, 4, 27, 37, 44, 55}

        # ADSR 2: Piano/bells
        # Includes: low piano, glockenspiel
        adsr_2_instruments = {9, 59}

        # Apply mappings based on sample data
        if game_inst in adsr_0_instruments:
            return 0
        if game_inst in adsr_1_instruments:
            return 1
        if game_inst in adsr_2_instruments:
            return 2

        # Default to ADSR 0 for unknown instruments (sustained envelope works for most)
        return 0

    def parse_midi(self, midi_file: str):
    
        try:
            mid = mido.MidiFile(midi_file)
        except FileNotFoundError:
            print(f"Error: File {midi_file} not found.")
            return

        self.seq.ticks_per_quarter = mid.ticks_per_beat
        
        max_ticks = 0

        for track in mid.tracks:
            abs_time = 0
            for msg in track:
                abs_time += msg.time
                max_ticks = max(max_ticks, abs_time)

        self.seq.total_ticks = max_ticks

        if self.quantize_grid is not None:
            # User-specified musical grid (e.g., 4=16th notes, 8=32nd notes)
            grid_size = self.seq.ticks_per_quarter // self.quantize_grid
            print(f"Using user-specified quantization: 1/{self.quantize_grid} note grid ({grid_size} ticks)")
        else:
            # Default: minimal quantization to N64 tick resolution (48 TPQN)
            # This prevents rounding errors during clock conversion
            grid_size = self.seq.ticks_per_quarter // 48
        
        if grid_size > 1:
            self._preprocess_quantize(mid, grid_size)

        # Occurs in MuseScore exported-midis with certain multi-patch instruments
        self._preprocess_remove_unused_pcs(mid)

        for i, midi_track in enumerate(mid.tracks):

            target_track = Track(name=f"Track {i}")

            cur_sources = []
            active_notes = {}

            last_note_ending_ticks = 0
            
            abs_time = 0
            
            # Track the MIDI channel for this track (detect from first channel message)
            track_channel = None
            is_drum_track = False

            # Track active notes (for note-off pairing)
            active_notes = {}  # note_num -> (start_time, velocity, gm_program)

            for msg in midi_track:

                abs_time += msg.time

                if abs_time > self.seq.total_ticks:
                    break
                
                # Detect MIDI channel from first message with channel
                if track_channel is None and hasattr(msg, 'channel'):
                    track_channel = msg.channel
                    is_drum_track = (track_channel == 9) # MIDI channel 10 (0-indexed = 9)

                if msg.type == 'program_change':

                    if self.use_gm_mapping:
                        # Map GM program to game instrument (use middle C = 60 for default range) (transposed an octave to fit HM64 samples)
                        game_inst = get_game_instrument_from_gm(msg.program, 60 - 12)
                        if not hasattr(target_track, 'gm_program'): 
                            target_track.gm_program = msg.program
                    else:
                        # No mapping: use MIDI program number directly as game instrument
                        game_inst = msg.program
                    
                    # Only set track.instrument if it's the first program change
                    if target_track.instrument == -1:
                        target_track.instrument = game_inst

                    if not hasattr(track, 'program_changes'):
                        target_track.program_changes = []

                    target_track.program_changes.append((abs_time, game_inst))

                elif msg.type == 'note_on' and msg.velocity > 0:
                    # Store with current GM program for later mapping
                    current_gm_program = target_track.instrument if not is_drum_track else 0
                    active_notes[msg.note] = (abs_time, msg.velocity / 127.0, current_gm_program)
                
                elif (msg.type == 'note_off') or (msg.type == 'note_on' and msg.velocity == 0):
                    
                    if msg.note in active_notes:

                        start_time, velocity, gm_program = active_notes[msg.note]
                        
                        # Add rest if there's a gap (skip tiny gaps from rounding)
                        gap = start_time - last_note_ending_ticks

                        if gap > 2:  # Only add rest if gap is more than 2 ticks
                            target_track.notes.append(NoteEvent(
                                type=NoteType.REST,
                                ticks=last_note_ending_ticks
                            ))
                        
                        transposed_note = max(0, msg.note - 12)

                        # Map to proper instrument depending on note value
                        if self.use_gm_mapping:
                            
                            if is_drum_track:
                                
                                # Drum track: map GM drum note to game percussion instrument
                                game_inst = get_game_drum_instrument(msg.note)

                                if game_inst is None:
                                    # Unknown drum; skip this note
                                    last_note_ending_ticks = abs_time
                                    del active_notes[msg.note]
                                    continue

                                if not hasattr(track, 'program_changes'):
                                    track.program_changes = []

                                track.program_changes.append((start_time, game_inst))

                                # FIXME: this note value should be based off the sample of the instrument patch 
                                # Using a fixed low note for now
                                transposed_note = 0x24  # C2

                            else:
                                game_inst = get_game_instrument_from_gm(gm_program, transposed_note)

                        duration = abs_time - start_time

                        target_track.notes.append(NoteEvent(
                            type=NoteType.NOTE,
                            ticks=start_time,
                            velocity=velocity,
                            note=transposed_note,
                            duration_ticks=duration
                        ))

                        last_note_ending_ticks = abs_time
                        del active_notes[msg.note]

                elif msg.type == 'control_change':

                    if msg.control == 7:  # Volume
                        idx = self._get_source_index(cur_sources, ControllerType.VOLUME)
                        cur_sources[idx].events.append(ControllerEvent(abs_time, msg.value / 127.0))
                    elif msg.control == 10:  # Pan
                        idx = self._get_source_index(cur_sources, ControllerType.PAN)
                        cur_sources[idx].events.append(ControllerEvent(abs_time, msg.value / 127.0))
                        
                elif msg.type == 'pitchwheel':

                    idx = self._get_source_index(cur_sources, ControllerType.FINE_PITCH)
                    # Convert from -8192..8191 to 0.0..1.0
                    val = (msg.pitch + 8192) / 16383.0
                    cur_sources[idx].events.append(ControllerEvent(abs_time, val))

                elif msg.type == 'set_tempo':

                    # Convert microseconds per beat to BPM
                    bpm = 60000000.0 / msg.tempo
                    idx = self._get_source_index(cur_sources, ControllerType.TEMPO)
                    # Normalize to 0-255 BPM range (storing as 0.0-1.0)
                    cur_sources[idx].events.append(ControllerEvent(abs_time, bpm / 255.0))

            # Post-process track
            if target_track.notes:
                # Add sources
                base_idx = len(self.seq.sources)
                for s in cur_sources:
                    s.owner_track_name = target_track.name
                self.seq.sources.extend(cur_sources)

                # Link sources
                for j, s in enumerate(cur_sources):
                    idx = base_idx + j
                    if s.type == ControllerType.VOLUME: target_track.volume_source = idx
                    elif s.type == ControllerType.PAN: target_track.pan_source = idx
                    elif s.type == ControllerType.FINE_PITCH: target_track.fine_pitch_source = idx
                    elif s.type == ControllerType.TEMPO: self.seq.tempo_source = idx

                # Default Vol
                if target_track.volume_source == -1:
                    self.seq.sources.append(ControllerSource(ControllerType.VOLUME, [ControllerEvent(0, 100/127.0)]))
                    target_track.volume_source = len(self.seq.sources) - 1

                if target_track.instrument == -1: target_track.instrument = 25 # Default Piano
                self.seq.tracks.append(target_track)

        # Recalc total ticks
        max_t = 0
        for t in self.seq.tracks:
             if t.notes:
                  last = t.notes[-1]
                  end = last.ticks + (last.duration_ticks if last.type == NoteType.NOTE else 0)
                  max_t = max(max_t, end)
        self.seq.total_ticks = max_t

    def _build_controller_stream(self, source_idx: int, normalize_center=False) -> bytearray:

        data = bytearray()

        if source_idx < 0: return data
        
        source = self.seq.sources[source_idx]
        
        if not source.events: return data
        
        for i, event in enumerate(source.events):

            val = int(event.value * 127) & 0x7F
            
            # Duration
            if i < len(source.events) - 1:
                duration = source.events[i+1].ticks - event.ticks
            else:
                duration = self.seq.total_ticks - event.ticks
            
            if duration == 1:
                data.append(val)
            else:
                data.append(val | 0x80)
                data.extend(self._encode_varlen(duration))

        return data

    def _build_track_data(self, track: Track, track_idx: int, enable_loop: bool = False) -> bytearray:

        data = bytearray()
        
        if enable_loop:
            data.append(OpCode.LOOP_START)
            data.append(0xFF)

        data.append(OpCode.SET_INSTRUMENT)
        data.extend(self._encode_varlen(track.instrument))

        data.append(OpCode.SET_SLIDE)
        data.append(0x00)
        
        data.append(OpCode.ADSR_INDEX)
        data.append(get_adsr_for_instrument(track.instrument))

        # Tempo (Master Track)
        if track_idx == 0 and self.seq.tempo_source >= 0:

            ts = self.seq.sources[self.seq.tempo_source]

            if ts.events:
                bpm = int(ts.events[0].value * 255)
                data.append(OpCode.SET_TEMPO)
                data.append(bpm)
        
        data.append(OpCode.RELEASE_TO_FULL)
        data.append(0x01)
        
        # set default effect/reverb (0x28 = 40; typically used value)
        data.append(OpCode.EFFECT)
        data.append(track.effect if track.effect else 0x28)

        # Analyze patterns for optimization
        velocities = [int(n.velocity * 127) for n in track.notes if n.type == NoteType.NOTE]

        durations = []

        for i, note in enumerate(track.notes):

            if note.type == NoteType.NOTE:
                # Find if this is the last note (ignoring any trailing rests)
                is_last_note = True
                for j in range(i + 1, len(track.notes)):
                    if track.notes[j].type == NoteType.NOTE:
                        is_last_note = False
                        break

                if is_last_note:
                    # Last note: use actual MIDI duration instead of extending to total_ticks
                    length = note.duration_ticks
                elif i < len(track.notes) - 1:
                    length = track.notes[i + 1].ticks - note.ticks
                else:
                    length = note.duration_ticks

                durations.append(length)

        # Set repeat velocity if beneficial
        use_defined_velocity = False
        defined_velocity = 0x7F

        if velocities and self._is_mostly_constant(velocities):
            common_vel = velocities[0]
            data.append(OpCode.SET_REPEAT_VEL)
            data.append(common_vel)
            use_defined_velocity = True
            defined_velocity = common_vel
        else:
            # Velocities vary - enable velocity in notes so each note can have its own velocity byte
            data.append(OpCode.VELOCITY_ENABLE) # Re-enable velocity in notes (each note will include velocity)
            use_defined_velocity = False

        # Set repeat length if beneficial (only if ALL lengths are the same and there are multiple notes)
        use_defined_length = False

        has_rests = any(n.type == NoteType.REST for n in track.notes)
    
        # If all durations are identical and there are at least 2 notes, then safe to use SET_DEFINED_LENGTH
        if durations and len(durations) > 1 and len(set(durations)) == 1 and not has_rests:
            common_len = durations[0]
            data.append(OpCode.SET_DEFINED_LENGTH)
            data.extend(self._encode_varlen(common_len))
            use_defined_length = True

        # Setup optimization vars
        program_map = {t: p for t, p in track.program_changes}
        cur_inst = track.instrument
        cur_effect = track.effect

        # Write Notes
        for i, note in enumerate(track.notes):

            # Check Program Change
            if note.ticks in program_map:

                new_inst = program_map[note.ticks]

                if new_inst != cur_inst:
                    data.append(OpCode.SET_INSTRUMENT)
                    data.extend(self._encode_varlen(new_inst))
                    data.append(OpCode.ADSR_INDEX)
                    data.append(get_adsr_for_instrument(new_inst))
                    cur_inst = new_inst
            
            # Check Multiplex Switching (Merged tracks)
            if track.instrument_timeline:

                # The notes list and timeline list are parallel arrays created in optimization.py
                # format: (ticks, instrument, effect, note_obj, source_idx)
                if i < len(track.instrument_timeline):

                    event_info = track.instrument_timeline[i]
                    new_inst = event_info[1]
                    new_effect = event_info[2]

                    if new_inst != cur_inst:
                        data.append(OpCode.SET_INSTRUMENT)
                        data.extend(self._encode_varlen(new_inst))
                        cur_inst = new_inst
                    
                    if new_effect != cur_effect:
                        data.append(OpCode.EFFECT)
                        data.append(new_effect)
                        cur_effect = new_effect

            # Dynamic instrument switching based on note range
            # (for multi-patch instruments like piano and strings)
            if note.type == NoteType.NOTE and is_multi_patch_instrument(cur_inst):
                
                # Calculate the transposed note value that will be written
                # note_val = max(0, note.note - 12)

                # Get the best instrument for this specific note
                best_instrument = get_best_instrument_for_note(cur_inst, note.note)

                if best_instrument != cur_inst:
                    # Switch instrument for this note
                    data.append(OpCode.SET_INSTRUMENT)
                    data.extend(self._encode_varlen(best_instrument))
                    cur_inst = best_instrument
                    # Update ADSR for new instrument
                    data.append(OpCode.ADSR_INDEX)
                    data.append(self._adsr_index_for_instrument(cur_inst))

            if note.type == NoteType.REST:

                # Rest = 0x60
                data.append(0x60)

                # Velocity (required even for rests in some formats)
                if not use_defined_velocity:
                    data.append(0x00)
                
                # Length
                if not use_defined_length:
                    if i < len(track.notes) - 1:
                        length = track.notes[i + 1].ticks - note.ticks
                    else:
                        length = self.seq.total_ticks - note.ticks
                    data.extend(self._encode_varlen(length))

            elif note.type == NoteType.NOTE:
                
                # Transpose & Clamp
                val = max(0, note.note - 12)
                
                # Clamp to max note
                if val >= 0x60: 
                    val = 0x5F
                
                data.append(val)
                
                vel = int(note.velocity * 127) & 0x7F

                if not use_defined_velocity:
                    data.append(vel)
                elif vel != defined_velocity:
                    # Value changed; need to update
                    data.insert(-1, OpCode.SET_REPEAT_VEL)  # Insert before note
                    data.insert(-1, vel)
                    defined_velocity = vel

                if not use_defined_length:
                    # For most notes: use time to next event to account for small gaps
                    # For last note: use actual duration to avoid artificial extension
                    is_last_note = (i == len(track.notes) - 1)

                    if not is_last_note:
                        # Use time to next event (preserves original timing behavior)
                        length = track.notes[i + 1].ticks - note.ticks
                    else:
                        # Last note: use actual MIDI duration instead of extending to total_ticks
                        length = note.duration_ticks

                    # Ensure at least 1 tick duration
                    if length < 1:
                        length = 1

                    data.extend(self._encode_varlen(length))

        if enable_loop:
            data.append(OpCode.LOOP_BACK)

        data.append(OpCode.END_OF_TRACK)

        return data

    def convert(self, midi_path: str, out_path: str, enable_loop: bool = False):
        
        print(f"Parsing {midi_path}...")
        self.parse_midi(midi_path)
        
        print("Converting clock base...")
        self.seq.convert_clock_base()
        
        print("Optimizing...")
        optimizer = SequenceOptimizer()
        optimizer.split_polyphony(self.seq)
        self.seq.optimize_all()
        optimizer.compress_tracks(self.seq)
        
        print("Trimming...")
        self.seq.trim_trailing_silence()
        self.seq.trim_events()
        self.seq.ensure_final_rests()
        
        print("Serializing...")
        final_data = bytearray()
        
        # Components
        track_data_blobs = [self._build_track_data(t, i, enable_loop) for i, t in enumerate(self.seq.tracks)]
        vol_blobs = [self._build_controller_stream(t.volume_source) for t in self.seq.tracks]
        pitch_blobs = [self._build_controller_stream(t.fine_pitch_source) for t in self.seq.tracks]
        
        header_size = 0x20
        num_tracks = len(self.seq.tracks)
        table_size = num_tracks * 4
        
        ptr_track = header_size
        ptr_vol = ptr_track + table_size
        ptr_pitch = ptr_vol + table_size
        
        # Actual data starts after all pointer tables
        data_start = ptr_pitch + table_size
        
        # ADSR Table placement
        # Align to 4 bytes for table placement
        adsr_align = (4 - (data_start % 4)) % 4
        adsr_offset = data_start + adsr_align
        data_start_actual = adsr_offset + len(ADSR_TABLE)
        
        # Write Header
        final_data.extend(struct.pack('>I', num_tracks))
        final_data.extend(struct.pack('>I', ptr_track))
        final_data.extend(struct.pack('>I', ptr_vol))
        final_data.extend(struct.pack('>I', ptr_pitch))
        final_data.extend(struct.pack('>I', adsr_offset)) # Drum/ADSR
        final_data.extend(struct.pack('>I', adsr_offset)) # ADSR
        final_data.extend(b'\x00' * 8) # Reserved
        
        # Pointers & Data Accumulation
        blob_data = bytearray()
        current_blob_offset = data_start_actual
        
        # Track Pointers
        for b in track_data_blobs:
            final_data.extend(struct.pack('>I', current_blob_offset))
            current_blob_offset += len(b)
            blob_data.extend(b)
            
        # Volume Pointers
        for b in vol_blobs:
            if b:
                final_data.extend(struct.pack('>I', current_blob_offset))
                current_blob_offset += len(b)
                blob_data.extend(b)
            else:
                final_data.extend(struct.pack('>I', 0))
                
        # Pitch Pointers
        for b in pitch_blobs:
            if b:
                final_data.extend(struct.pack('>I', current_blob_offset))
                current_blob_offset += len(b)
                blob_data.extend(b)
            else:
                final_data.extend(struct.pack('>I', 0))
                
        # Write ADSR table now (after pointer tables), aligned to 4 bytes
        final_data.extend(b'\x00' * adsr_align)
        final_data.extend(ADSR_TABLE)
        
        final_data.extend(blob_data)
        
        print(f"Writing {len(final_data)} bytes to {out_path}")
        with open(out_path, 'wb') as f:
            f.write(final_data)