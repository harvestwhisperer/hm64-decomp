import struct
from typing import List

from typedefs import TimeValue 
from constants import OpCode

class SequenceParser:
    def __init__(self, data: bytes, log_filename=None, verbose=False):
        self.data = data
        self.log_file = open(log_filename, 'w') if log_filename else None
        self.verbose = verbose

    def __del__(self):
        if self.log_file:
            self.log_file.close()

    def _log(self, msg):
        if self.verbose:
            print(msg)
        if self.log_file:
            self.log_file.write(msg + "\n")

    def read_u16_be(self, offset: int) -> int:
        """Read unsigned 16-bit big-endian integer"""
        return struct.unpack('>H', self.data[offset:offset+2])[0]
    
    def read_u32_be(self, offset):
        return struct.unpack('>I', self.data[offset:offset+4])[0]
    
    def get_variable_length(self, offset: int) -> tuple[int, int]:
        """
        Read variable-length value and return (value, bytes_consumed)
        If < 0x80: single byte
        If >= 0x80: two bytes with 7+8 bit encoding
        """
        first_byte = self.data[offset]

        if first_byte < 0x80:
            return (first_byte, 1)
        else:
            # Two byte encoding: 7 bits from first byte + 8 bits from second byte
            second_byte = self.data[offset + 1]
            value = ((first_byte & 0x7F) << 8) | second_byte
            return (value, 2)
    
    def parse_header(self):
        """Parse the sequence header"""
        
        self._log("=" * 80)
        self._log("HEADER INFORMATION")
        self._log("=" * 80)
        
        self.num_tracks = self.read_u32_be(0x00)
        self.track_pointer = self.read_u32_be(0x04)
        self.volume_pointer = self.read_u32_be(0x08)
        self.pitch_bend_pointer = self.read_u32_be(0x0C)
        self.drum_pointer = self.read_u32_be(0x10)
        self.adsr_pointer = self.read_u32_be(0x14)
        
        self._log(f"Number of tracks: {self.num_tracks}")
        self._log(f"Track pointer: 0x{self.track_pointer:08X}")
        self._log(f"Volume pointer: 0x{self.volume_pointer:08X}")
        self._log(f"Pitch bend pointer: 0x{self.pitch_bend_pointer:08X}")
        self._log(f"Drum pointer: 0x{self.drum_pointer:08X}")
        self._log(f"ADSR pointer: 0x{self.adsr_pointer:08X}")
        self._log("")
        
        # Validate header
        if not self._validate_header():
            self._log("=" * 80)
            self._log("WARNING: Header validation failed!")
            self._log("This file may be using a different seq format.")
            self._log("=" * 80)
            self._log("")
    
    def _validate_header(self) -> bool:
        """Check if header values look reasonable"""
        
        file_size = len(self.data)
        
        # Check if track count is reasonable (1-50 tracks is typical)
        if self.num_tracks == 0 or self.num_tracks > 100:
            self._log(f"Unusual track count: {self.num_tracks}")
            return False
        
        # Check if pointers are within file bounds
        if self.track_pointer >= file_size:
            self._log(f"Track pointer 0x{self.track_pointer:08X} exceeds file size")
            return False
        
        # The header is 24 bytes (0x18), so track_pointer should be >= 0x18
        # Track pointer points to the track pointer table, not track data
        if self.track_pointer > 0 and self.track_pointer < 0x18:
            self._log(f"Track pointer 0x{self.track_pointer:08X} overlaps header")
            return False
        
        # Verify pointer ordering makes sense
        # track_pointer < volume_pointer < pitch_bend_pointer (when non-zero)
        if self.volume_pointer > 0 and self.track_pointer >= self.volume_pointer:
            self._log(f"Track pointer >= volume pointer (unusual ordering)")
            return False
        
        return True
    
    def parse_pitch_bend_stream(self, track_num: int, start: int, end: int) -> List[TimeValue]:
        """Parse pitch bend envelope data"""

        if start == 0 or start >= len(self.data):
            return []
        
        self._log(f"\n--- Pitch Bend Stream for Track {track_num} ---")
        self._log(f"Start: 0x{start:08X}, End: 0x{end:08X}")
        
        pitch_bends = []
        absolute_time = 0
        spot = start
        
        while spot < end and spot < len(self.data):

            pitch_byte = self.data[spot]
            single_length = not (pitch_byte & 0x80)
            pitch = pitch_byte & 0x7F
            signed_pitch = pitch - 0x40  # Center at 0x40
            spot += 1
            
            length = 1
            if not single_length:
                length, bytes_consumed = self.get_variable_length(spot)
                
                if bytes_consumed == 1:
                    self._log(f"  0x{spot-1:08X} Time: {absolute_time:6d} | Pitch: {pitch:3d} ({signed_pitch:+4d}) | Length: {length:4d} [0x{self.data[spot]:02X}]")
                else:
                    self._log(f"  0x{spot-1:08X} Time: {absolute_time:6d} | Pitch: {pitch:3d} ({signed_pitch:+4d}) | Length: {length:4d} [0x{self.data[spot]:02X}{self.data[spot+1]:02X}]")
                spot += bytes_consumed
            else:
                self._log(f"  0x{spot-1:08X} Time: {absolute_time:6d} | Pitch: {pitch:3d} ({signed_pitch:+4d}) | Length: 1 [single]")
            
            pitch_bends.append(TimeValue(pitch, absolute_time, absolute_time + length))
            absolute_time += length
        
        return pitch_bends
    
    def parse_volume_stream(self, track_num: int, start: int, end: int) -> List[TimeValue]:
        """Parse volume envelope data"""

        if start == 0 or start >= len(self.data):
            return []
        
        self._log(f"\n--- Volume Stream for Track {track_num} ---")
        self._log(f"Start: 0x{start:08X}, End: 0x{end:08X}")
        
        volumes = []
        absolute_time = 0
        spot = start
        
        while spot < end and spot < len(self.data):

            volume_byte = self.data[spot]
            single_length = not (volume_byte & 0x80)
            volume = volume_byte & 0x7F
            spot += 1
            
            length = 1
            if not single_length:
                length, bytes_consumed = self.get_variable_length(spot)
                
                if bytes_consumed == 1:
                    self._log(f"  0x{spot-1:08X} Time: {absolute_time:6d} | Volume: {volume:3d} (0x{volume:02X}) | Length: {length:4d} [0x{self.data[spot]:02X}]")
                else:
                    self._log(f"  0x{spot-1:08X} Time: {absolute_time:6d} | Volume: {volume:3d} (0x{volume:02X}) | Length: {length:4d} [0x{self.data[spot]:02X}{self.data[spot+1]:02X}]")
                spot += bytes_consumed
            else:
                self._log(f"  0x{spot-1:08X} Time: {absolute_time:6d} | Volume: {volume:3d} (0x{volume:02X}) | Length: 1 [single]")
            
            volumes.append(TimeValue(volume, absolute_time, absolute_time + length))
            absolute_time += length
        
        return volumes

    def parse(self):
        """Main parsing function."""

        self.parse_header()
        
        # Parse all tracks
        for track_idx in range(self.num_tracks):

            # Get track data pointer
            track_data_ptr = self.read_u32_be(self.track_pointer + (track_idx * 4))
            
            if track_data_ptr == 0:
                self._log(f"\nTrack {track_idx}: EMPTY (pointer = 0x00000000)")
                continue
            
            # Calculate track end
            if track_idx < self.num_tracks - 1:
                track_end = self.read_u32_be(self.track_pointer + ((track_idx + 1) * 4))
                if track_end == 0:
                    track_end = len(self.data)
            else:
                track_end = len(self.data)
            
            # Get volume data pointers
            volume_ptr = self.read_u32_be(self.volume_pointer + (track_idx * 4))
            volume_end = 0
            
            if volume_ptr != 0:

                # Find end of volume data by looking for next non-zero volume pointer
                for y in range(track_idx + 1, self.num_tracks):
                    test_ptr = self.read_u32_be(self.volume_pointer + (y * 4))
                    if test_ptr != 0:
                        volume_end = test_ptr
                        break
                
                if volume_end == 0:
                    # No more volume data found; prefer the start of the first pitch-bend stream if present, else end of file
                    first_pitch_ptr = 0
                    if self.pitch_bend_pointer > 0:
                        for y in range(self.num_tracks):
                            test_ptr = self.read_u32_be(self.pitch_bend_pointer + (y * 4))
                            if test_ptr != 0:
                                first_pitch_ptr = test_ptr
                                break
                    if first_pitch_ptr != 0:
                        volume_end = first_pitch_ptr
                    else:
                        volume_end = len(self.data)
            
            # Get pitch bend pointers (skip if pointer section is 0)
            pitch_ptr = 0
            pitch_end = 0
            
            if self.pitch_bend_pointer > 0:

                pitch_ptr = self.read_u32_be(self.pitch_bend_pointer + (track_idx * 4))
                
                if pitch_ptr != 0:
                    # Find end by looking for next non-zero pitch bend pointer
                    for y in range(track_idx + 1, self.num_tracks):
                        test_ptr = self.read_u32_be(self.pitch_bend_pointer + (y * 4))
                        if test_ptr != 0:
                            pitch_end = test_ptr
                            break
                    
                    if pitch_end == 0:
                        # No more pitch bend data found; use end of file
                        pitch_end = len(self.data)
            
            volumes = self.parse_volume_stream(track_idx, volume_ptr, volume_end)
            pitch_bends = self.parse_pitch_bend_stream(track_idx, pitch_ptr, pitch_end)
            
            self.parse_track(track_data_ptr, track_end)

    def parse_track(self, start: int, end: int):

        spot = start
        cmd_offset = spot

        absolute_time = 0

        use_defined_length = False
        defined_length = 0
        
        use_defined_velocity = False
        defined_velocity = 0x7F

        while spot < end and spot < len(self.data):

            command = self.data[spot]
            spot += 1
            
            # Note or rest
            if command < 0x80:

                # Get velocity
                if use_defined_velocity:
                    velocity = defined_velocity
                    vel_str = f"[defined: {velocity}]"
                else:
                    vel_byte = self.data[spot]
                    velocity = vel_byte & 0x7F
                    
                    if vel_byte & 0x80:
                        use_defined_velocity = True
                        defined_velocity = velocity
                        vel_str = f"{velocity} [SET REPEAT]"
                    else:
                        vel_str = f"{velocity}"

                    spot += 1
                
                # Get length
                if use_defined_length:
                    note_length = defined_length
                    len_str = f"[defined: {note_length}]"
                else:
                    note_length, bytes_consumed = self.get_variable_length(spot)
                    if bytes_consumed == 1:
                        len_str = f"{note_length} [0x{self.data[spot]:02X}]"
                    else:
                        len_str = f"{note_length} [0x{self.data[spot]:02X}{self.data[spot+1]:02X}]"
                    
                    spot += bytes_consumed
                
                note = command

                # Determine note type
                if note == 0x60:
                    note_str = "REST"
                elif note < 0x60:
                    note_name = ['C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B'][note % 12]
                    octave = note // 12
                    note_str = f"NOTE {note_name}{octave} (0x{note:02X})"
                else:
                    note_str = f"UNKNOWN NOTE (0x{note:02X})"
                
                self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | {note_str:20s} | Vel: {vel_str:15s} | Len: {len_str}")
                
                absolute_time += note_length

            # Commands (0x80 and above)
            else:
                if command == OpCode.END_OF_TRACK:
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | END OF TRACK")
                    break
                
                elif command == OpCode.SET_INSTRUMENT:
                    instrument, bytes_consumed = self.get_variable_length(spot)
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | SET INSTRUMENT: {instrument}")
                    spot += bytes_consumed
                
                elif command == OpCode.SET_SLIDE:
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | SET SLIDE: 0x{self.data[spot]:02X}")
                    spot += 1
                
                elif command == OpCode.ADSR_SET:
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | ADSR SET (7 bytes)")
                    spot += 7
                
                elif command == OpCode.SET_TEMPO:
                    tempo = self.data[spot]
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | SET TEMPO: {tempo} BPM")
                    spot += 1
                
                elif command == OpCode.SET_RELEASE_TIME:
                    release_time = self.read_u16_be(spot)
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | SET RELEASE TIME: {release_time}")
                    spot += 2
                
                elif command == OpCode.RELEASE_TO_FULL:
                    use_defined_release = False
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | RELEASE TIME BACK TO FULL: 0x{self.data[spot]:02X}")
                    spot += 1
                
                elif command == OpCode.VIBRATO_UP:
                    delay, amount, speed = self.data[spot], self.data[spot+1], self.data[spot+2]
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | VIBRATO UP: Delay={delay}, Amount={amount}, Speed={speed}")
                    spot += 3
                
                elif command == OpCode.VIBRATO_DOWN:
                    delay, amount, speed = self.data[spot], self.data[spot+1], self.data[spot+2]
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | VIBRATO DOWN: Delay={delay}, Amount={amount}, Speed={speed}")
                    spot += 3
                
                elif command == OpCode.SOLO:
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | SOLO")
                
                elif command == OpCode.SET_DEFINED_LENGTH:

                    length, bytes_consumed = self.get_variable_length(spot)
                    
                    if length == 0:
                        use_defined_length = False
                        self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | DISABLE DEFINED LENGTH")
                    else:
                        use_defined_length = True
                        defined_length = length
                        self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | SET DEFINED LENGTH: {length}")
                    
                    spot += bytes_consumed
                
                elif command == OpCode.TRANSPOSE:
                    transpose = struct.unpack('b', bytes([self.data[spot]]))[0]  # signed
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | TRANSPOSE: {transpose:+d}")
                    spot += 1
                
                elif command == OpCode.DETUNE:
                    detune = self.data[spot]
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | DETUNE: {detune}")
                    spot += 1
                
                elif command == OpCode.ADSR_INDEX:
                    adsr_index = self.data[spot]
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | ADSR INDEX: {adsr_index}")
                    spot += 1
                
                elif command == OpCode.ENV_TRIGGER_OFF:
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | ENVELOPE TRIGGER OFF")
                
                elif command == OpCode.ENV_TRIGGER_ON:
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | ENVELOPE TRIGGER ON")
                
                elif command == OpCode.SAMPLE_TRIGGER_OFF:
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | SAMPLE TRIGGER OFF")
                
                elif command == OpCode.SAMPLE_TRIGGER_ON:
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | SAMPLE TRIGGER ON")
                
                elif command == OpCode.LOOP_START:
                    loop_count = self.data[spot]
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | LOOP START: Count={loop_count}")
                    spot += 1
                
                elif command == OpCode.LOOP_BACK:
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | LOOP BACK")
                    break
                
                elif command == OpCode.WOBBLE:
                    offset, transpose_cnt, normal_cnt = self.data[spot], self.data[spot+1], self.data[spot+2]
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | WOBBLE: Offset={offset}, Transpose={transpose_cnt}, Normal={normal_cnt}")
                    spot += 3
                
                elif command == OpCode.VELOCITY_ENABLE:
                    use_defined_velocity = False
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | REENABLE VELOCITY IN NOTES")
                
                elif command == OpCode.VELOCITY_DEFINE:
                    use_defined_velocity = True
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | ENABLE VELOCITY DEFINED IN NOTES")
                
                elif command == OpCode.SET_REPEAT_VEL:
                    defined_velocity = self.data[spot]
                    use_defined_velocity = True
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | SET REPEAT VELOCITY: {defined_velocity}")
                    spot += 1
                
                elif command == OpCode.PAN:
                    pan = self.data[spot]
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | PAN: {pan} (0x{pan:02X})")
                    spot += 1
                
                elif command == OpCode.DRUM_MAP_ON:
                    drum_offset = self.data[spot]
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | DRUM MAP ON: Offset={drum_offset}")
                    spot += 1
                
                elif command == OpCode.DRUM_MAP_OFF:
                    current_drum_offset = -1
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | DRUM MAP OFF")
                
                elif command == OpCode.EFFECT:
                    effect = self.data[spot]
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | EFFECT: {effect}")
                    spot += 1
                
                elif command == OpCode.SET_MIN_MAX_TRANS:
                    min_t, max_t = self.data[spot], self.data[spot+1]
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | SET MIN/MAX TRANSPOSE: Min={min_t}, Max={max_t}")
                    spot += 2
                
                elif command == OpCode.SET_MIN_MAX_VOL:
                    min_v, max_v = self.data[spot], self.data[spot+1]
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | SET MIN/MAX VOLUME: Min={min_v}, Max={max_v}")
                    spot += 2
                
                elif command == OpCode.SET_MIN_MAX_PAN:
                    min_p, max_p = self.data[spot], self.data[spot+1]
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | SET MIN/MAX PAN: Min={min_p}, Max={max_p}")
                    spot += 2
                
                elif command == OpCode.NOTE_VOL:
                    volume = self.data[spot]
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | SET NOTE VOLUME: {volume}")
                    spot += 1
                
                elif command == OpCode.LOAD_TRACK:
                    length, bytes_consumed = self.get_variable_length(spot)
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | LOAD TRACK #: Length={length}")
                    spot += bytes_consumed
                
                elif command == OpCode.PITCH_BEND:
                    pitch = self.data[spot]
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | PITCH BEND: {pitch}")
                    spot += 1
                
                elif command == OpCode.STEREO_SWEEP:
                    speed = self.data[spot]
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | STEREO SWEEP SPEED: {speed}")
                    spot += 1
                
                elif command == OpCode.MASTER_EFFECT:
                    effect = self.data[spot]
                    effect_names = {0: "None", 1: "Small Room", 2: "Big Room", 3: "Chorus", 4: "Flange", 5: "Echo"}
                    effect_str = effect_names.get(effect, f"Unknown ({effect})")
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | MASTER TRACK EFFECT: {effect_str}")
                    spot += 1
                
                elif command == OpCode.MASTER_SYNC:
                    sync = self.data[spot]
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | MASTER TRACK SYNC: {sync}")
                    spot += 1
                
                elif command == OpCode.REENABLE_LENGTH:
                    use_defined_length = False
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | REENABLE LENGTH ON NOTES")
                
                elif command == 0xB0:
                    val = self.data[spot]
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | UNKNOWN 0xB0: {val}")
                    spot += 1
                
                else:
                    self._log(f"0x{cmd_offset:08X} @ {absolute_time:6d} | UNKNOWN COMMAND: 0x{command:02X}")
                    self._log(f"ERROR: Stopping track parse due to unknown command")
                    break