import argparse
import csv
import json
import numpy as np
import os
import re
import sys

from pathlib import Path

ROM_PATH = Path(__file__).parent / "../../baserom.us.z64"
CSV_PATH = Path(__file__).parent / "text_addresses.csv"

VERBOSE = "--verbose" in sys.argv
LITERAL_MODE = "--literal" in sys.argv  # Output exact control codes for recompilation

rom = None

def set_rom():
    global rom
    rom = memoryview(ROM_PATH.read_bytes())

CHAR_MAP = {

    # --- Row 1 (Hiragana) ---
    0x0B: 'あ', 0x0C: 'い', 0x0D: 'う', 0x0E: 'え', 0x0F: 'お',
    0x10: 'か', 0x11: 'き', 0x12: 'く', 0x13: 'け', 0x14: 'こ',
    0x15: 'さ', 0x16: 'し', 0x17: 'す', 0x18: 'せ', 0x19: 'そ', 0x1A: 'た',

    # --- Row 2 ---
    0x1B: 'ち', 0x1C: 'つ', 0x1D: 'て', 0x1E: 'と', 0x1F: 'な',
    0x20: 'に', 0x21: 'ぬ', 0x22: 'ね', 0x23: 'の', 0x24: 'は',
    0x25: 'ひ', 0x26: 'ふ', 0x27: 'へ', 0x28: 'ほ', 0x29: 'ま', 0x2A: 'み',

    # --- Row 3 ---
    0x2B: 'む', 0x2C: 'め', 0x2D: 'も', 0x2E: 'や', 0x2F: 'ゆ',
    0x30: 'よ', 0x31: 'ら', 0x32: 'り', 0x33: 'る', 0x34: 'れ',
    0x35: 'ろ', 0x36: 'わ', 0x37: 'を', 0x38: 'ん', 0x39: 'が', 0x3A: 'ぎ',

    # --- Row 4 ---
    0x3B: 'ぐ', 0x3C: 'げ', 0x3D: 'ご', 0x3E: 'ざ', 0x3F: 'じ',
    0x40: 'ず', 0x41: 'ぜ', 0x42: 'ぞ', 0x43: 'だ', 0x44: 'ぢ',
    0x45: 'づ', 0x46: 'で', 0x47: 'ど', 0x48: 'ば', 0x49: 'び', 0x4A: 'ぶ',

    # --- Row 5 ---
    0x4B: 'べ', 0x4C: 'ぼ', 0x4D: 'ぱ', 0x4E: 'ぴ', 0x4F: 'ぷ',
    0x50: 'ぺ', 0x51: 'ぽ', 0x52: 'ゃ', 0x53: 'ゅ', 0x54: 'ょ',
    0x55: 'っ', 0x56: 'ぁ', 0x57: 'ぃ', 0x58: 'ぅ', 0x59: 'ぇ', 0x5A: 'ぉ',

    # --- Row 6 (Katakana Start) ---
    0x5B: 'ア', 0x5C: 'イ', 0x5D: 'ウ', 0x5E: 'エ', 0x5F: 'オ',
    0x60: 'カ', 0x61: 'キ', 0x62: 'ク', 0x63: 'ケ', 0x64: 'コ',
    0x65: 'サ', 0x66: 'シ', 0x67: 'ス', 0x68: 'セ', 0x69: 'ソ', 0x6A: 'タ',

    # --- Row 7 ---
    0x6B: 'チ', 0x6C: 'ツ', 0x6D: 'テ', 0x6E: 'ト', 0x6F: 'ナ',
    0x70: 'ニ', 0x71: 'ヌ', 0x72: 'ネ', 0x73: 'ノ', 0x74: 'ハ',
    0x75: 'ヒ', 0x76: 'フ', 0x77: 'ヘ', 0x78: 'ホ', 0x79: 'マ', 0x7A: 'ミ',

    # --- Row 8 ---
    0x7B: 'ム', 0x7C: 'メ', 0x7D: 'モ', 0x7E: 'ヤ', 0x7F: 'ユ',
    0x80: 'ヨ', 0x81: 'ラ', 0x82: 'リ', 0x83: 'ル', 0x84: 'レ',
    0x85: 'ロ', 0x86: 'ワ', 0x87: 'ヲ', 0x88: 'ン', 0x89: 'ガ', 0x8A: 'ギ',

    # --- Row 9 ---
    0x8B: 'グ', 0x8C: 'ゲ', 0x8D: 'ゴ', 0x8E: 'ザ', 0x8F: 'ジ',
    0x90: 'ズ', 0x91: 'ゼ', 0x92: 'ゾ', 0x93: 'ダ', 0x94: 'ヂ',
    0x95: 'ヅ', 0x96: 'デ', 0x97: 'ド', 0x98: 'バ', 0x99: 'ビ', 0x9A: 'ブ',

    # --- Row 10 ---
    0x9B: 'ベ', 0x9C: 'ボ', 0x9D: 'パ', 0x9E: 'ピ', 0x9F: 'プ',
    0xA0: 'ペ', 0xA1: 'ポ', 0xA2: 'ヴ', 0xA3: 'ャ', 0xA4: 'ュ',
    0xA5: 'ョ', 0xA6: 'ッ', 0xA7: 'ァ', 0xA8: 'ィ', 0xA9: 'ゥ', 0xAA: 'ェ',

    0xAB: 'ォ',

    0xAC: 'A',
    0xAD: 'B', 
    0xAE: 'C',
    0xAF: 'D',
    0xB0: 'E',
    0xB1: 'F',
    0xB2: 'G',
    0xB3: 'H',
    0xB4: 'I',
    0xB5: 'J',
    0xB6: 'K',
    0xB7: 'L',
    0xB8: 'M',
    0xB9: 'N',
    0xBA: 'O',
    0xBB: 'P',
    0xBC: 'Q',
    0xBD: 'R',
    0xBE: 'S',
    0xBF: 'T',
    0xC0: 'U',
    0xC1: 'V',
    0xC2: 'W',
    0xC3: 'X',
    0xC4: 'Y',
    0xC5: 'Z',
    0xC6: 'a',
    0xC7: 'b',
    0xC8: 'c',
    0xC9: 'd',
    0xCA: 'e',
    0xCB: 'f',
    0xCC: 'g',
    0xCD: 'h',
    0xCE: 'i',
    0xCF: 'j',
    0xD0: 'k',
    0xD1: 'l',
    0xD2: 'm',
    0xD3: 'n',
    0xD4: 'o',
    0xD5: 'p',
    0xD6: 'q',
    0xD7: 'r',
    0xD8: 's',
    0xD9: 't',
    0xDA: 'u',
    0xDB: 'v',
    0xDC: 'w',
    0xDD: 'x',
    0xDE: 'y',
    0xDF: 'z',
    0xE0: '1',
    0xE1: '2',
    0xE2: '3',
    0xE3: '4',
    0xE4: '5',
    0xE5: '6',
    0xE6: '7',
    0xE7: '8',
    0xE8: '9',
    0xE9: '0',
    0xEA: '?',
    0xEB: '!', 
    0xEC: '-',
    0xEF: ',',
    0xF0: '.',
    0xF1: '/',
    0xF2: '☆', 
    0xF3: '★', 
    0xF4: '&',
    0xF5: '♡', 
    0xF6: '♥', 
    0xF7: 'Ა', # tear drop; alternative: 💧
    0xF8: '∴', # paw print; alternative: 🐾
    0xF9: ' ',
    0xFA: '※',   
    0xFB: '○', 
    0xFC: '℃', 
    0xFD: '春', 
    0xFE: '夏', 
    0xFF: '秋', 
    0x100: '冬', 
    0x101: '—',
    0x102: '×',
    0x103: ':',
    0x104: ';', 
    0x105: '…', 
    0x106: "‥",
    0x107: '“',  
    0x108: '”', 
    # 0x109: '', # left diagonal unused 
    # 0x10A: '', # right diagonal unused

    # --- Row 17 ---
    0x10B: '(', 0x10C: ")", 0x10D: '♪', 0x10E: '’', 0x10F: '日', 
    0x110: '月', 0x111: '¥',  0x112: '%', 0x113: '#', 0x114: '+', 
    0x115: '朝', 0x116: '昼', 0x117: '夕', 0x118: '夜', 0x119: '時', 0x11A: '間',

    # --- Row 18 ---
    0x11B: '分', 0x11C: '年', 0x11D: '昔', 0x11E: '昨', 0x11F: '火', 
    0x120: '水', 0x121: '木', 0x122: '山', 0x123: '川', 0x124: '池', 
    0x125: '海', 0x126: '風', 0x127: '星', 0x128: '空', 0x129: '光', 0x12A: '町',

    # --- Row 19 ---
    0x12B: '村', 0x12C: '店', 0x12D: '父', 0x12E: '母', 0x12F: '様', 
    0x130: '兄', 0x131: '弟', 0x132: '姉', 0x133: '妹', 0x134: '男', 
    0x135: '女', 0x136: '歩', 0x137: '走', 0x138: '買', 0x139: '売', 0x13A: '入',

    # --- Row 20 ---
    0x13B: '出', 0x13C: '大', 0x13D: '中', 0x13E: '小', 0x13F: '多', 
    0x140: '少', 0x141: '長', 0x142: '左', 0x143: '右', 0x144: '前', 
    0x145: '弱', 0x146: '西', 0x147: '南', 0x148: '北', 0x149: '上', 0x14A: '下',

    # --- Row 21 ---
    0x14B: '高', 0x14C: '犬', 0x14D: '牛', 0x14E: '馬', 0x14F: '鳥', 
    0x150: '羊', 0x151: '虫', 0x152: '貝', 0x153: '目', 0x154: '毛', 
    0x155: '体', 0x156: '歯', 0x157: '頭', 0x158: '首', 0x159: '声', 0x15A: '顔',

    # --- Row 22 ---
    0x15B: '手', 0x15C: '足', 0x15D: '生', 0x15E: '死', 0x15F: '命', 
    0x160: '花', 0x161: '畑', 0x162: '野', 0x163: '菜', 0x164: '葉', 
    0x165: '草', 0x166: '芽', 0x167: '茶', 0x168: '薬', 0x169: '種', 0x16A: '植',

    # --- Row 23 ---
    0x16B: '農', 0x16C: '刈', 0x16D: '料', 0x16E: '理', 0x16F: '動', 
    0x170: '物', 0x171: '感', 0x172: '謝', 0x173: '牧', 0x174: '場', 
    0x175: '結', 0x176: '婚', 0x177: '温', 0x178: '泉', 0x179: '天', 0x17A: '国',

    # --- Row 24 ---
    0x17B: '勉', 0x17C: '強', 0x17D: '一', 0x17E: '二', 0x17F: '十', 
    0x180: '万', 0x181: '王', 0x182: '力', 0x183: '工', 0x184: '方', 
    0x185: '不', 0x186: '丈', 0x187: '夫', 0x188: '友', 0x189: '午', 0x18A: '元',

    # --- Row 25 ---
    0x18B: '才', 0x18C: '太', 0x18D: '以', 0x18E: '古', 0x18F: '用', 
    0x190: '可', 0x191: '先', 0x192: '代', 0x193: '失', 0x194: '切', 
    0x195: '反', 0x196: '戸', 0x197: '々', 0x198: '予', 0x199: '内', 0x19A: '匹',

    # --- Row 26 ---
    0x19B: '化', 0x19C: '示', 0x19D: '平', 0x19E: '礼', 0x19F: '半', 
    0x1A0: '去', 0x1A1: '辺', 0x1A2: '止', 0x1A3: '付', 0x1A4: '外', 
    0x1A5: '正', 0x1A6: '仕', 0x1A7: '収', 0x1A8: '幻', 0x1A9: '由', 0x1AA: '本',

    # --- Row 27 ---
    0x1AB: '今', 0x1AC: '必', 0x1AD: '行', 0x1AE: '早', 0x1AF: '自', 
    0x1B0: '名', 0x1B1: '守', 0x1B2: '休', 0x1B3: '好', 0x1B4: '汗', 
    0x1B5: '同', 0x1B6: '肉', 0x1B7: '当', 0x1B8: '曲', 0x1B9: '学', 0x1BA: '来',

    # --- Row 28 ---
    0x1BB: '会', 0x1BC: '式', 0x1BD: '舟', 0x1BE: '台', 0x1BF: '羽', 
    0x1C0: '団', 0x1C1: '回', 0x1C2: '対', 0x1C3: '世', 0x1C4: '次', 
    0x1C5: '安', 0x1C6: '何', 0x1C7: '伝', 0x1C8: '全', 0x1C9: '忘', 0x1CA: '近',

    # --- Row 29 ---
    0x1CB: '返', 0x1CC: '役', 0x1CD: '初', 0x1CE: '利', 0x1CF: '材', 
    0x1D0: '冷', 0x1D1: '決', 0x1D2: '芸', 0x1D3: '見', 0x1D4: '作', 
    0x1D5: '言', 0x1D6: '向', 0x1D7: '赤', 0x1D8: '幸', 0x1D9: '注', 0x1DA: '洗',

    # --- Row 30 ---
    0x1DB: '消', 0x1DC: '泣', 0x1DD: '活', 0x1DE: '波', 0x1DF: '法', 
    0x1E0: '供', 0x1E1: '価', 0x1E2: '的', 0x1E3: '度', 0x1E4: '実', 
    0x1E5: '糸', 0x1E6: '苦', 0x1E7: '身', 0x1E8: '形', 0x1E9: '岩', 0x1EA: '定',

    # --- Row 31 ---
    0x1EB: '字', 0x1EC: '放', 0x1ED: '表', 0x1EE: '和', 0x1EF: '枚', 
    0x1F0: '林', 0x1F1: '車', 0x1F2: '固', 0x1F3: '君', 0x1F4: '別', 
    0x1F5: '具', 0x1F6: '香', 0x1F7: '事', 0x1F8: '明', 0x1F9: '家', 0x1FA: '然',

    # --- Row 32 ---
    0x1FB: '話', 0x1FC: '美', 0x1FD: '每', 0x1FE: '荷', 0x1FF: '常', 
    0x200: '栄', 0x201: '性', 0x202: '取', 0x203: '原', 0x204: '係', 
    0x205: '神', 0x206: '味', 0x207: '屋', 0x208: '計', 0x209: '信', 0x20A: '始',

    # --- Row 33 ---
    0x20B: '点', 0x20C: '面', 0x20D: '迷', 0x20E: '科', 0x20F: '品', 
    0x210: '急', 0x211: '逃', 0x212: '相', 0x213: '指', 0x214: '単', 
    0x215: '界', 0x216: '青', 0x217: '要', 0x218: '旅', 0x219: '教', 0x21A: '紅',

    # --- Row 34 ---
    0x21B: '通', 0x21C: '道', 0x21D: '記', 0x21E: '設', 0x21F: '愛', 
    0x220: '部', 0x221: '新', 0x222: '能', 0x223: '集', 0x224: '真', 
    0x225: '個', 0x226: '食', 0x227: '恋', 0x228: '週', 0x229: '業', 0x22A: '読',

    # --- Row 35 ---
    0x22B: '地', 0x22C: '負', 0x22D: '直', 0x22E: '奴', 0x22F: '彼', 
    0x230: '考', 0x231: '狩', 0x232: '歌', 0x233: '使', 0x234: '育', 
    0x235: '館', 0x236: '絵', 0x237: '笑', 0x238: '島', 0x239: '箱', 0x23A: '都',

    # --- Row 36 ---
    0x23B: '飲', 0x23C: '持', 0x23D: '開', 0x23E: '師', 0x23F: '祭', 
    0x240: '塩', 0x241: '病', 0x242: '算', 0x243: '飼', 0x244: '待', 
    0x245: '語', 0x246: '紙', 0x247: '室', 0x248: '悲', 0x249: '運', 0x24A: '姫',

    # --- Row 37 ---
    0x24B: '息', 0x24C: '特', 0x24D: '根', 0x24E: '落', 0x24F: '関', 
    0x250: '意', 0x251: '勝', 0x252: '乳', 0x253: '雲', 0x254: '届', 
    0x255: '描', 0x256: '値', 0x257: '遠', 0x258: '探', 0x259: '級', 0x25A: '数',

    # --- Row 38 ---
    0x25B: '答', 0x25C: '着', 0x25D: '競', 0x25E: '景', 0x25F: '積', 
    # 0x262 is empty
    0x260: '情', 0x261: '聞',  0x263: '私', 0x264: '図', 
    0x265: '書', 0x266: '所', 0x267: '思', 0x268: '心', 0x269: '配', 0x26A: '季',

    # --- Row 39 ---
    0x26B: '節', 0x26C: '雨', 0x26D: '雪', 0x26E: '白', 0x26F: '知', 
    0x270: '子', 0x271: '主', 0x272: '人', 0x273: '口', 0x274: '耳', 
    0x275: '立', 0x276: '石', 0x277: '甘', 0x278: '土', 0x279: '色', 0x27A: '広',

    # --- Row 40 ---
    0x27B: '合', 0x27C: '音', 0x27D: '金', 0x27E: '魚', 0x27F: '酒', 
    0x280: '帰', 0x281: '娘', 0x282: '親', 0x283: '橋', 0x284: '客', 
    0x285: '後', 0x286: '夢', 0x287: '他', 0x288: '楽', 0x289: '番', 0x28A: '園',

    # --- Row 41 ---
    0x28B: '商', 0x28C: '者', 0x28D: '発', 0x28E: '奥', 0x28F: '敗', 
    0x290: '校', 0x291: '器', 0x292: '油', 0x293: '写', 0x294: '第', 
    0x295: '①', 0x296: '②', 0x297: '③', 0x298: '④', 0x299: '⑤', 0x29A: '⑥',

    # --- Row 42 ---
    0x29B: '⑦', 0x29C: '⑧', 0x29D: '⑨', 0x29E: '↖', 0x29F: '↘', 
    0x2A0: '↗', 0x2A1: '↙',
}

CONTROL_CODES = {
    0: 'LINEBREAK',
    1: 'SOFTBREAK',
    2: 'TEXTEND',
    3: 'WAIT',  # Followed by 1-byte duration parameter
    4: 'WAIT_WITH_ICON',
    5: 'LOAD_TEXT',  # Followed by 2-byte text index
    6: 'RESERVED',
    7: 'INSERT_GAMEVAR', # Followed by 1-byte string index
    8: 'WAIT_VARIANT',
    9: 'CHARACTER_AVATAR', # Followed by 1-byte animation index
}

BIT_MASKS = [0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01]

def get_offset_array(index_start, index_end):

    if rom is None:
        set_rom()

    num_entries = int((index_end - index_start) // 4)

    offsets = np.frombuffer(rom, dtype=np.dtype(">u4"), count=num_entries, offset=index_start)

    # handle offset arrays that have 0 padding at the end

    # get indices of all non-zero elements 
    nonzeros = np.nonzero(offsets)[0]
    # get last non-zero index
    last = nonzeros[-1] if len(nonzeros) > 0 else 0
    
    # trim original array
    return offsets[:last+1]

def set_text_segments(index_start: int, index_end: int, text_start: int) -> dict:

    # Text data ends at the start of its own index table
    text_end = index_start

    # Maximum valid offset - offsets at or past this point to the index table
    max_valid_offset = index_start - text_start

    if VERBOSE == True:
        print(f"Text bank layout:")
        print(f"  Index: 0x{index_start:08X} - 0x{index_end:08X}")
        print(f"  Text:  0x{text_start:08X} - 0x{text_end:08X}")
        print(f"  Max valid offset: 0x{max_valid_offset:04X}")

    index_size = index_end - index_start
    index_count = index_size // 4

    if VERBOSE == True:
        print(f"  Index size: {index_size} bytes, {index_count} entries")

    offsets = get_offset_array(index_start, index_end)

    # Calculate start addresses and sizes for each valid text segment
    segments = []

    for idx, offset in enumerate(offsets):

        start_address = text_start + offset

        # Calculate size by getting difference between segments or between text_end and last segment
        if idx + 1 < len(offsets):
            next_offset = offsets[idx + 1]
            # Cap next_offset at max_valid_offset to avoid reading into index table
            if next_offset > max_valid_offset:
                next_offset = max_valid_offset
            next_start = text_start + next_offset
            size = next_start - start_address
        else:
            # Last segment goes to the start of the index table
            size = text_end - start_address

        # Mark segments that point at or past the text data end as terminators
        # These are kept for index table generation but have no extractable text
        is_terminator = offset >= max_valid_offset

        if size < 0:
            if VERBOSE == True:
                print(f"  WARNING: Segment {idx} has invalid size {size} bytes")
            size = 0

        if VERBOSE == True:
            term_str = " (terminator)" if is_terminator else ""
            print(f"  Segment {idx} (index {idx}): 0x{start_address:08X}, size {size} bytes{term_str}")

        segments.append({
            'index': idx,
            'start_address': start_address,
            'size': size,
            'offset': offset,
            'is_terminator': is_terminator
        })
    
    return {
        'index_count': index_count,
        'segments': segments,
        'index_start': index_start,
        'index_end': index_end,
        'text_start': text_start,
        'text_end': text_end
    }

class TextDecoder:

    def __init__(self):
        self.position = 0
        self.char_counter = 0
        self.control_byte = 0
        
    def decode_stream(self, byte_data, literal_mode=False):
        """
        Decode a byte stream into a list of decoded items.
        
        Args:
            byte_data: Bytes, hex string, or list of bytes
            literal_mode: If True, parse entire segment including content after TEXTEND
                         (needed for byte-perfect round-trip recompilation).
                         If False, stop at TEXTEND for human-readable output.
        """

        if isinstance(byte_data, str):
            # Convert hex string to bytes
            byte_data = bytes.fromhex(byte_data.replace(' ', ''))

        elif isinstance(byte_data, list):
            byte_data = bytes(byte_data)
            
        self.position = 0
        self.char_counter = 0
        self.control_byte = 0
        self._current_data = byte_data  # Store reference for parameter extraction
        result = []
        
        while self.position < len(byte_data):

            decoded_value = self._decode_next_character(byte_data)
            
            if decoded_value is not None:
                result.append(decoded_value)

                # In non-literal mode, stop at TEXTEND for readability
                # In literal mode, continue parsing to capture trailing content for byte-matching
                if not literal_mode and isinstance(decoded_value, dict):
                    if decoded_value.get('type') == 'control' and decoded_value.get('name') == 'TEXTEND':
                        if VERBOSE:
                            remaining = byte_data[self.position:]
                            print(f"    Stopping at TEXTEND. Remaining bytes: {remaining.hex()}")
                        break

                # Check if this is a segment-ending control code (legacy behavior)
                if isinstance(decoded_value, dict) and decoded_value.get('segment_end'):
                    if VERBOSE == True:
                        print(f"    Decode stopped at WAITINPUT at position {self.position}/{len(byte_data)}")
                    break
            
            else:

                if VERBOSE == True:
                
                    # stop if next character can't be decoded
                    print(f"    Decode stopped at position {self.position}/{len(byte_data)}")
                    
                    # Show the last few bytes for debugging
                    start_pos = max(0, self.position - 10)
                    end_pos = min(len(byte_data), self.position + 10)
                    context_bytes = byte_data[start_pos:end_pos]
                    
                    print(f"    Context bytes around position {self.position}: {context_bytes.hex()}")

                break
                
        return result
    
    def _decode_next_character(self, data):

        if self.position >= len(data):
            if VERBOSE == True:
                print(f"    EOF reached at position {self.position}")
            return None
            
        # Read new control byte every 8 characters
        if self.char_counter % 8 == 0:

            if self.position >= len(data):
                if VERBOSE == True:
                    print(f"    EOF while reading control byte at position {self.position}")
                return None

            self.control_byte = data[self.position]
            
            if VERBOSE == True:
                print(f"    Char {self.char_counter}: New control byte 0x{self.control_byte:02X} at position {self.position}")
            
            self.position += 1
            
        if self.position >= len(data):
            if VERBOSE == True:
                print(f"    EOF after control byte at position {self.position}")
            return None
            
        # Check if this character position needs 2-byte encoding
        bit_index = self.char_counter % 8
        needs_two_bytes = (self.control_byte & BIT_MASKS[bit_index]) != 0
        
        if VERBOSE == True:
            print(f"    Char {self.char_counter}: bit_index={bit_index}, needs_two_bytes={needs_two_bytes}")
        
        if needs_two_bytes:
            
            # Read 2 bytes for control command
            if self.position + 1 >= len(data):
                # Not enough data for 2-byte read - this is an edge case at segment end
                # Output remaining byte; round-trip check will catch if this causes issues
                if self.position < len(data):
                    remaining_byte = data[self.position]
                    self.position += 1
                    self.char_counter += 1
                    if VERBOSE == True:
                        print(f"    Incomplete 2-byte read at segment end, byte 0x{remaining_byte:02X}")
                    return f'[CHAR:0x{remaining_byte:02X}]'
                return None

            byte1 = data[self.position]
            byte2 = data[self.position + 1]

            self.position += 2

            # byteswap 16-bit values
            result = byte1 | (byte2 << 8) 
            
            if result in CHAR_MAP:
                decoded = CHAR_MAP[result]
                if VERBOSE == True:
                    print(f"    Read 2-byte character: 0x{result:04X} -> '{decoded}'")

            elif result == 0:
                if VERBOSE == True:
                    print(f"    Found null terminator (0x0000), ending decode")
                return None

            else:
                # Invalid 2-byte code - preserve as [WORD:0xXXXX] for round-trip
                # The transpiler will encode this as a 2-byte value (control bit set)
                decoded = f'[WORD:0x{result:04X}]'
                if VERBOSE == True:
                    print(f"    Invalid 2-byte code 0x{result:04X} -> [WORD:0x{result:04X}]")
                
        else:
            # Read 1 byte
            byte_val = data[self.position]
            self.position += 1
    
            result = byte_val
            decoded = self._decode_character(result)
    
            if VERBOSE == True:
                print(f"    Read 1-byte character: 0x{result:02X} -> {decoded}")
            
        self.char_counter += 1
        
        # sanity check if addresses are messed up
        if self.char_counter > 5000:
            print(f"    ERROR: Processed {self.char_counter} characters - likely incorrect segment size")
            print(f"    Current position: {self.position}/{len(data)} bytes")
            return None
            
        return decoded
    
    def _extract_parameter(self, control_code):
        """Extract parameter bytes that follow certain control codes"""

        if VERBOSE == True:
            print(f"      Extracting parameter for control code {control_code}")
        
        if control_code == 5:  # LOAD_TEXT - 2-byte little-endian parameter
        
            if hasattr(self, '_current_data') and self.position + 1 < len(self._current_data):
                byte1 = self._current_data[self.position]
                byte2 = self._current_data[self.position + 1]
                self.position += 2
                # DON'T increment char_counter here - it messes up control byte timing
                param = byte1 | (byte2 << 8)  # Little endian
    
                if VERBOSE == True:
                    print(f"      Extracted 2-byte parameter: 0x{param:04X}")
    
                return param    
        
        elif control_code in [3, 7, 9]:  # WAIT, INSERT_GAMEVAR, CHAR_ANIMATION - 1-byte parameter

            if hasattr(self, '_current_data') and self.position < len(self._current_data):

                param = self._current_data[self.position]
                self.position += 1

                # don't increment char_counter here

                if VERBOSE == True:
                    print(f"      Extracted 1-byte parameter: 0x{param:02X}")

                return param

        return None
    
    def _decode_character(self, value):

        if value in CONTROL_CODES:

            control_name = CONTROL_CODES[value]

            # Extract parameter if this control code needs one
            param = self._extract_parameter(value)

            if param is not None:
                return {'type': 'control', 'code': value, 'name': control_name, 'parameter': param}
            else:
                return {'type': 'control', 'code': value, 'name': control_name}
            
        elif value in CHAR_MAP:
            return {'type': 'character', 'value': CHAR_MAP[value]}
        else:
            # Show both the raw value and after -0xB
            font_index = value - 0xB if value >= 0xB else value
            return {'type': 'unknown', 'raw': value, 'font_index': font_index}
    
    def decode_and_format(self, byte_data, literal_mode=False):
        """
        Decode and format as readable text with enhanced control code handling.
        
        Args:
            byte_data: The binary data to decode
            literal_mode: If True, output exact control codes and parse entire segment
                         (for byte-perfect recompilation).
                         If False, stop at TEXTEND and use friendly formatting with 
                         newlines (for human reading).
        """

        decoded = self.decode_stream(byte_data, literal_mode=literal_mode)
        result = []
        
        for item in decoded:
        
            if isinstance(item, dict):
        
                if item['type'] == 'control':
        
                    if 'parameter' in item:
                        result.append(f"[{item['name']}:{item['parameter']:02X}]")
                    else:
                        if literal_mode:
                            # Literal mode: just the control code, no extra formatting
                            result.append(f"[{item['name']}]")
                        else:
                            # Friendly mode: natural newlines, implicit TEXTEND
                            if item['name'] == 'LINEBREAK':
                                result.append('\n')  # Natural newline
                            elif item['name'] == 'SOFTBREAK':
                                result.append('\n')  # Natural newline (soft)
                            elif item['name'] == 'TEXTEND':
                                pass  # Implicit - omit in friendly mode
                            else:
                                result.append(f"[{item['name']}]")
        
                elif item['type'] == 'character':
                    result.append(item['value'])
        
                elif item['type'] == 'unknown':
                    result.append(f"[CHAR:0x{item['raw']:02X}→0x{item['font_index']:02X}]")
        
            else:
                # Fallback for old format
                result.append(str(item))
        
        return ''.join(result)

def decode_text_bank(index_start: int, index_end: int, text_start: int, literal_mode: bool = False) -> list:
    """
    Decode a text bank from ROM.
    
    Args:
        index_start: ROM address of index table start
        index_end: ROM address of index table end  
        text_start: ROM address of text data start
        literal_mode: If True, output exact control codes for recompilation.
                     If False, output friendly format with newlines for reading.
    """
    
    if rom is None:
        set_rom()
    
    bank_info = set_text_segments(index_start, index_end, text_start)
    decoder = TextDecoder()
    decoded_texts = []
    
    for segment in bank_info['segments']:

        # Skip terminator segments - they mark the end of text data for index calculation
        # but have no actual text content to decode
        if segment.get('is_terminator', False):
            if VERBOSE == True:
                print(f"Skipping terminator segment {segment['index']}: offset 0x{segment['offset']:04X}")
            decoded_texts.append({
                'index': segment['index'],
                'start_address': f"0x{segment['start_address']:08X}",
                'size': 0,
                'actual_size': 0,
                'offset': segment['offset'],
                'is_terminator': True,
                'decoded_text': '',
                'raw_data': ''
            })
            continue

        if VERBOSE == True:
            print(f"Processing segment {segment['index']}: 0x{segment['start_address']:08X}, calculated size: {segment['size']}")

        segment_data = rom[segment['start_address']:segment['start_address'] + segment['size']]
        actual_size = len(segment_data)

        if VERBOSE == True:
            print(f"  Actual segment_data size: {actual_size} bytes")
            if actual_size >= 16:
                print(f"  Last 16 bytes: {segment_data[-16:].hex()}")
                print(f"  First 16 bytes: {segment_data[:16].hex()}")

        # Check if segment is all zeros (padding, not actual text content)
        # These segments exist in the index but contain only padding bytes
        if actual_size > 0 and all(b == 0 for b in segment_data):
            if VERBOSE:
                print(f"  Segment {segment['index']} is padding ({actual_size} bytes of zeros)")
            decoded_texts.append({
                'index': segment['index'],
                'start_address': f"0x{segment['start_address']:08X}",
                'size': segment['size'],
                'actual_size': actual_size,
                'offset': segment['offset'],
                'is_padding': True,
                'decoded_text': '',
                'raw_data': segment_data[:50].hex()
            })
            continue

        try:

            if VERBOSE == True:
                print(f"  Starting decode for segment {segment['index']}...")

            decoded = decoder.decode_and_format(segment_data, literal_mode=literal_mode)

            # Check if this is a mid-stream loaded segment that won't round-trip correctly
            # Two indicators: starts with [CHAR:0xXX], or re-encoding produces different bytes
            needs_rawbytes = False

            # Check 1: starts with [CHAR:0xXX] pattern
            if re.match(r'^\[CHAR:0x[0-9A-Fa-f]{2}\]', decoded):
                needs_rawbytes = True

            # Check 2: try re-encoding and compare (only for small segments to avoid performance issues)
            if not needs_rawbytes and len(segment_data) <= 64:
                try:
                    from hm64_text_transpiler import TextEncoder
                    test_encoder = TextEncoder()
                    test_encoder.auto_textend = False
                    test_encoder.convert_newlines = False
                    # Strip trailing padding from comparison
                    reencoded = test_encoder.encode_text(decoded)
                    original_trimmed = bytes(segment_data).rstrip(b'\x00')
                    reencoded_trimmed = reencoded.rstrip(b'\x00')
                    if original_trimmed != reencoded_trimmed:
                        needs_rawbytes = True
                        if VERBOSE:
                            print(f"  Segment {segment['index']} round-trip mismatch: {original_trimmed.hex()} vs {reencoded_trimmed.hex()}")
                except Exception as e:
                    if VERBOSE:
                        print(f"  Could not verify round-trip for segment {segment['index']}: {e}")

            if needs_rawbytes:
                # Use RAWBYTES directive with commented interpretation
                byte_interpretation = []
                for b in segment_data:
                    if b in CONTROL_CODES:
                        byte_interpretation.append(f'[{CONTROL_CODES[b]}]')
                    elif b in CHAR_MAP:
                        byte_interpretation.append(CHAR_MAP[b])
                    else:
                        byte_interpretation.append(f'[0x{b:02X}]')
                interpretation = ''.join(byte_interpretation)
                decoded = f'[RAWBYTES:{segment_data.hex()}]\n# Byte interpretation: {interpretation}'
                if VERBOSE == True:
                    print(f"  Segment {segment['index']} converted to RAWBYTES (mid-stream loaded)")

            if VERBOSE == True:
                print(f"  Successfully decoded segment {segment['index']}")

            decoded_texts.append({
                'index': segment['index'],
                'start_address': f"0x{segment['start_address']:08X}",
                'size': segment['size'],
                'actual_size': actual_size,
                'offset': segment['offset'],
                'decoded_text': decoded,
                'raw_data': segment_data[:50].hex() + ('...' if segment['size'] > 50 else '')
            })

        except Exception as e:
            print(f"  Error decoding segment {segment['index']}: {e}")
            decoded_texts.append({
                'index': segment['index'],
                'start_address': f"0x{segment['start_address']:08X}",
                'size': segment['size'],
                'actual_size': actual_size,
                'offset': segment['offset'],
                'error': str(e),
                'raw_data': segment_data[:50].hex() + ('...' if segment['size'] > 50 else '')
            })
    
    return decoded_texts

def write_text_bank_to_files(index_start: int, index_end: int, text_start: int, output_dir: str = None, literal_mode: bool = False) -> str:
    """
    Extract a text bank from ROM and write to individual text files.
    
    Args:
        index_start: ROM address of index table start
        index_end: ROM address of index table end
        text_start: ROM address of text data start
        output_dir: Output directory name (optional)
        literal_mode: If True, output exact control codes for recompilation.
                     If False, output friendly format with newlines for reading.
    """
    if rom is None:
        set_rom()
    
    if output_dir is None:
        output_dir = f"text-block-0x{text_start:08X}"
    
    assets_path = Path(__file__).parent.parent.parent / "assets" / "text"
    output_path = assets_path / output_dir
    
    output_path.mkdir(parents=True, exist_ok=True)
    
    decoded_texts = decode_text_bank(index_start, index_end, text_start, literal_mode=literal_mode)

    files_written = 0
    for text_info in decoded_texts:

        # Skip terminator segments - they have no text content to write
        if text_info.get('is_terminator', False):
            if VERBOSE:
                print(f"Skipping terminator segment {text_info['index']} (no file created)")
            continue

        # Skip padding segments - all zeros, no text content
        if text_info.get('is_padding', False):
            if VERBOSE:
                print(f"Skipping padding segment {text_info['index']} ({text_info['size']} bytes of zeros)")
            continue

        filename = f"text{text_info['index']:03d}.txt"
        file_path = output_path / filename

        with open(file_path, 'w', encoding='utf-8') as f:

            f.write(f"# Text Segment {text_info['index']}\n")
            f.write(f"# Address: {text_info['start_address']}\n")
            f.write(f"# Size: {text_info['size']} bytes\n")
            f.write(f"# Raw data: {text_info['raw_data']}\n")
            f.write("# " + "="*60 + "\n")

            if 'error' in text_info:
                f.write(f"ERROR: {text_info['error']}\n")
            else:
                f.write(text_info['decoded_text'])

        files_written += 1

    # Write bank metadata file with terminator info for the transpiler
    metadata_path = output_path / "_metadata.txt"
    with open(metadata_path, 'w', encoding='utf-8') as f:
        f.write("# Text Bank Metadata\n")
        f.write(f"# INDEX_START: 0x{index_start:08X}\n")
        f.write(f"# INDEX_END: 0x{index_end:08X}\n")
        f.write(f"# TEXT_START: 0x{text_start:08X}\n")
        f.write(f"# TOTAL_SEGMENTS: {len(decoded_texts)}\n")
        f.write("\n")

        # List terminator segments (segments with no text content that mark end of data)
        terminators = [t for t in decoded_texts if t.get('is_terminator', False)]
        if terminators:
            f.write("# Terminator segments (index table entries with no text data):\n")
            for term in terminators:
                f.write(f"TERMINATOR: {term['index']}\n")
            f.write("\n")

        # List padding segments (all-zero segments, transpiler emits .space N)
        padding_segments = [t for t in decoded_texts if t.get('is_padding', False)]
        if padding_segments:
            f.write("# Padding segments (all zeros, no text file generated):\n")
            for pad in padding_segments:
                f.write(f"PADDING: {pad['index']} SIZE: {pad['size']}\n")

    print(f"Wrote {files_written} text files to directory: {output_path.absolute()}")

    return str(output_path.absolute())

def load_text_addresses() -> list:

    if not CSV_PATH.exists():
        print(f"CSV file not found: {CSV_PATH}")
        return []
    
    text_banks = []

    with open(CSV_PATH, 'r', newline='', encoding='utf-8') as csvfile:

        reader = csv.DictReader(csvfile)
        
        for row in reader:

            try:
                index_start = int(row['index_start'], 16)
                index_end = int(row['index_end'], 16)
                text_start = int(row['text_start'], 16)
                game_index = int(row['game_index']) if 'game_index' in row else 0
                
                text_banks.append({
                    'name': row['name'].strip(),
                    'index_start': index_start,
                    'index_end': index_end,
                    'text_start': text_start,
                    'game_index': game_index
                })
                
            except (ValueError, KeyError) as e:
                print(f"Error parsing CSV row {row}: {e}")
                continue
    
    return text_banks

def process_all_text_banks(command: str = 'write_files'):

    text_banks = load_text_addresses()

    if not text_banks:
        print("No text banks found in CSV file.")
        return
    
    if VERBOSE == True:
        print(f"Found {len(text_banks)} text banks in {CSV_PATH}")
    
    for bank in text_banks:

        if VERBOSE == True:
            print(f"\n--- Processing '{bank['name']}' ---")
            print(f"Index: 0x{bank['index_start']:08X} - 0x{bank['index_end']:08X}")
            print(f"Text:  0x{bank['text_start']:08X} - 0x{bank['index_start']:08X}")
        
        try:

            if command == 'write_files':
                output_path = write_text_bank_to_files(bank['index_start'], bank['index_end'], bank['text_start'], bank['name'], literal_mode=LITERAL_MODE)
                print(f"Files written to: {output_path}")
                
            elif command == 'decode_bank':
                decoded_texts = decode_text_bank(bank['index_start'], bank['index_end'], bank['text_start'], literal_mode=LITERAL_MODE)
                
            elif command == 'analyze_bank':
                set_rom()
                bank_info = set_text_segments(bank['index_start'], bank['index_end'], bank['text_start'])
                
                if VERBOSE == True:
                    print(f"  Total segments: {bank_info['index_count']}")
                
        except Exception as e:
            print(f"Error processing '{bank['name']}': {e}")

def main():

    if len(sys.argv) < 2:
        print("Usage:")
        print("  python hm64_text_utilities.py extract_bank <bank_name> [--literal]")
        print("  python hm64_text_utilities.py decode_bank <index_start> <index_end> <text_start> [--literal]")
        print("  python hm64_text_utilities.py analyze_bank <index_start> <index_end> <text_start>")
        print("  python hm64_text_utilities.py write_files <index_start> <index_end> <text_start> [output_dir] [--literal]")
        print("  python hm64_text_utilities.py process_all [write_files|decode_bank|analyze_bank] [--literal]")
        print("  python hm64_text_utilities.py list_banks")
        print("")
        print("Options:")
        print("  --verbose   Enable verbose output")
        print("  --literal   Output exact control codes for recompilation (vs friendly for reading)")
        sys.exit(1)

    cmd = sys.argv[1]

    if cmd == "extract_bank":

        # Extract a single bank by name (looks up addresses in CSV)
        if len(sys.argv) >= 3:
            
            bank_name = sys.argv[2]
            
            text_banks = load_text_addresses()
            
            if not text_banks:
                print("ERROR: Could not load text bank addresses from CSV")
                sys.exit(1)
            
            # Find the requested bank
            bank_info = None
            for bank in text_banks:
                if bank['name'] == bank_name:
                    bank_info = bank
                    break
            
            if bank_info is None:
                print(f"ERROR: Text bank '{bank_name}' not found in CSV")
                print("Available banks:")
                for bank in text_banks:
                    print(f"  - {bank['name']}")
                sys.exit(1)
            
            if VERBOSE:
                print(f"Extracting text bank '{bank_name}':")
                print(f"  Index: 0x{bank_info['index_start']:08X} - 0x{bank_info['index_end']:08X}")
                print(f"  Text:  0x{bank_info['text_start']:08X}")
            
            output_path = write_text_bank_to_files(
                bank_info['index_start'],
                bank_info['index_end'],
                bank_info['text_start'],
                bank_name,
                literal_mode=LITERAL_MODE
            )

            print(f"Extracted to: {output_path}")
            
        else:
            print("Please provide a bank name")
            print("Use 'list_banks' to see available banks")
            sys.exit(1)

    elif cmd == "decode_bank":

        if len(sys.argv) >= 5:

            index_start = int(sys.argv[2], 16)
            index_end = int(sys.argv[3], 16)
            text_start = int(sys.argv[4], 16)
            
            print(f"Decoding text bank:")
            print(f"Index: 0x{index_start:08X} - 0x{index_end:08X}")
            print(f"Text:  0x{text_start:08X}")
            
            decoded_texts = decode_text_bank(index_start, index_end, text_start, literal_mode=LITERAL_MODE)
            
            for text_info in decoded_texts:
                
                print(f"\n--- Text {text_info['index']} ---")
                print(f"Address: {text_info['start_address']}")
                print(f"Size: {text_info['size']} bytes")
                
                if 'error' in text_info:
                    print(f"Error: {text_info['error']}")
                else:
                    print(f"Text: {text_info['decoded_text']}")

                print(f"Raw data: {text_info['raw_data']}")
                
        else:
            print("Please provide index_start, index_end, and text_start addresses in hex")
            sys.exit(1)
            
    elif cmd == "analyze_bank":

        if len(sys.argv) >= 5:

            index_start = int(sys.argv[2], 16)
            index_end = int(sys.argv[3], 16)
            text_start = int(sys.argv[4], 16)
            
            set_rom()
            bank_info = set_text_segments(index_start, index_end, text_start)
            
            print(f"Text Bank Analysis:")
            print(f"Index: 0x{index_start:08X} - 0x{index_end:08X}")
            print(f"Text:  0x{text_start:08X}")
            print(f"Total segments: {bank_info['index_count']}")
            print(f"\nSegment breakdown:")
            
            for segment in bank_info['segments']:
                print(f"  {segment['index']:3d}: 0x{segment['start_address']:08X} (offset +0x{segment['offset']:04X}) - {segment['size']:4d} bytes")
                
        else:
            print("Please provide index_start, index_end, and text_start addresses in hex")
            sys.exit(1)
            
    elif cmd == "write_files":

        if len(sys.argv) >= 5:
        
            index_start = int(sys.argv[2], 16)
            index_end = int(sys.argv[3], 16)
            text_start = int(sys.argv[4], 16)
            
            # Optional custom directory name
            output_dir = sys.argv[5] if len(sys.argv) >= 6 else None
            
            print(f"Writing text bank files:")
            print(f"Index: 0x{index_start:08X} - 0x{index_end:08X}")
            print(f"Text:  0x{text_start:08X}")
            
            output_path = write_text_bank_to_files(index_start, index_end, text_start, output_dir, literal_mode=LITERAL_MODE)
            print(f"Files written to: {output_path}")
            
        else:
            print("Please provide index_start, index_end, and text_start addresses in hex")
            sys.exit(1)
            
    elif cmd == "process_all":

        command = sys.argv[2] if len(sys.argv) >= 3 else 'write_files'
        
        if command not in ['write_files', 'decode_bank', 'analyze_bank']:
            print(f"Unknown command for process_all: {command}")
            print("Available commands: write_files, decode_bank, analyze_bank")
            sys.exit(1)

        process_all_text_banks(command)
        
    elif cmd == "list_banks":

        text_banks = load_text_addresses()

        if not text_banks:
            print("No text banks found in CSV file.")
        else:
            print(f"Text banks found in {CSV_PATH}:")
            for i, bank in enumerate(text_banks):
                game_idx = f"(game:{bank['game_index']:02X})" if 'game_index' in bank else ""
                print(f"  {i+1:2d}. {bank['name']:15s} {game_idx:10s} Index: 0x{bank['index_start']:08X}-0x{bank['index_end']:08X} Text: 0x{bank['text_start']:08X}-0x{bank['index_start']:08X}")
    
    else:
        print(f"Unknown command: {cmd}")
        print("Available commands: extract_bank, decode_bank, analyze_bank, write_files, process_all, list_banks")

if __name__ == "__main__":
    main()