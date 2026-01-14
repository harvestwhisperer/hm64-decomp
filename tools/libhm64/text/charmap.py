"""
Character encoding tables for HM64 text.

Contains:
- CHAR_MAP: Byte value to character mapping
- CONTROL_CODES: Text control code definitions
- GAMEVAR_MACROS: Game variable string indices
- CHARACTER_AVATAR_MACROS: Character avatar indices
"""

from typing import Dict

# Character map: byte value -> display character
CHAR_MAP: Dict[int, str] = {
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

    # --- Letters A-Z ---
    0xAC: 'A', 0xAD: 'B', 0xAE: 'C', 0xAF: 'D', 0xB0: 'E',
    0xB1: 'F', 0xB2: 'G', 0xB3: 'H', 0xB4: 'I', 0xB5: 'J',
    0xB6: 'K', 0xB7: 'L', 0xB8: 'M', 0xB9: 'N', 0xBA: 'O',
    0xBB: 'P', 0xBC: 'Q', 0xBD: 'R', 0xBE: 'S', 0xBF: 'T',
    0xC0: 'U', 0xC1: 'V', 0xC2: 'W', 0xC3: 'X', 0xC4: 'Y', 0xC5: 'Z',

    # --- Letters a-z ---
    0xC6: 'a', 0xC7: 'b', 0xC8: 'c', 0xC9: 'd', 0xCA: 'e',
    0xCB: 'f', 0xCC: 'g', 0xCD: 'h', 0xCE: 'i', 0xCF: 'j',
    0xD0: 'k', 0xD1: 'l', 0xD2: 'm', 0xD3: 'n', 0xD4: 'o',
    0xD5: 'p', 0xD6: 'q', 0xD7: 'r', 0xD8: 's', 0xD9: 't',
    0xDA: 'u', 0xDB: 'v', 0xDC: 'w', 0xDD: 'x', 0xDE: 'y', 0xDF: 'z',

    # --- Numbers ---
    0xE0: '1', 0xE1: '2', 0xE2: '3', 0xE3: '4', 0xE4: '5',
    0xE5: '6', 0xE6: '7', 0xE7: '8', 0xE8: '9', 0xE9: '0',

    # --- Punctuation ---
    0xEA: '?', 0xEB: '!', 0xEC: '-', 0xEF: ',', 0xF0: '.', 0xF1: '/',

    # --- Symbols ---
    0xF2: '☆', 0xF3: '★', 0xF4: '&', 0xF5: '♡', 0xF6: '♥',
    0xF7: 'Ა',  # tear drop
    0xF8: '∴',  # paw print
    0xF9: ' ',
    0xFA: '※', 0xFB: '○', 0xFC: '℃',

    # --- Seasons ---
    0xFD: '春', 0xFE: '夏', 0xFF: '秋', 0x100: '冬',

    # --- More punctuation ---
    0x101: '—', 0x102: '×', 0x103: ':', 0x104: ';',
    0x105: '…', 0x106: "‥", 0x107: '\u201c', 0x108: '\u201d',
    # 0x109: '', # left diagonal unused
    # 0x10A: '', # right diagonal unused

    # --- Row 17 ---
    0x10B: '(', 0x10C: ")", 0x10D: '♪', 0x10E: "\u2019", 0x10F: '日',
    0x110: '月', 0x111: '¥', 0x112: '%', 0x113: '#', 0x114: '+',
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
    0x260: '情', 0x261: '聞', 0x263: '私', 0x264: '図',
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


# Control codes
CONTROL_CODES: Dict[int, str] = {
    0: 'LINEBREAK',
    1: 'SOFTBREAK',
    2: 'TEXTEND',
    3: 'WAIT',            # Followed by 1-byte duration parameter
    4: 'WAIT_WITH_ICON',
    5: 'LOAD_TEXT',       # Followed by 2-byte text index
    6: 'RESERVED',
    7: 'INSERT_GAMEVAR',  # Followed by 1-byte string index
    8: 'WAIT_VARIANT',
    9: 'CHARACTER_AVATAR',  # Followed by 1-byte animation index
}

# Reverse mapping
CONTROL_CODE_TO_BYTE = {v: k for k, v in CONTROL_CODES.items()}

# Game Variable String macros - maps index to semantic name
GAMEVAR_MACROS: Dict[int, str] = {
    0x00: 'PLAYER_NAME',
    0x01: 'FARM_NAME',
    0x02: 'DOG_NAME',
    0x03: 'HORSE_NAME',
    0x04: 'BABY_NAME',
    0x05: 'FARM_ANIMAL_1_NAME',
    0x06: 'FARM_ANIMAL_2_NAME',
    0x07: 'FARM_ANIMAL_3_NAME',
    0x08: 'FARM_ANIMAL_4_NAME',
    0x09: 'FARM_ANIMAL_5_NAME',
    0x0A: 'FARM_ANIMAL_6_NAME',
    0x0B: 'FARM_ANIMAL_7_NAME',
    0x0C: 'FARM_ANIMAL_8_NAME',
    0x0D: 'CURRENT_ANIMAL_NAME',
    0x0E: 'BEST_COW_NAME',
    0x0F: 'WIFE_NAME',
    0x10: 'UNUSED_10',
    0x11: 'TOOL_USE_COUNTER',
    0x12: 'PRICE',
    0x13: 'DEAD_ANIMAL_NAME',
    0x14: 'CURRENT_SEASON_NAME',
    0x15: 'DATE_NUMBER',
    0x16: 'WIFE_NAME_2',
    0x17: 'FODDER_AMOUNT',
    0x18: 'LUMBER_AMOUNT',
    0x19: 'CHICKEN_1_NAME',
    0x1A: 'CHICKEN_2_NAME',
    0x1B: 'CHICKEN_3_NAME',
    0x1C: 'CHICKEN_4_NAME',
    0x1D: 'CHICKEN_5_NAME',
    0x1E: 'CHICKEN_6_NAME',
    0x1F: 'HORSE_RACE_MEDALS',
    0x20: 'HARVEST_KING_NAME',
    0x21: 'NUMBER_21',
    0x22: 'UNUSED_22',
    0x23: 'UNUSED_23',
    0x24: 'UNUSED_24',
    0x25: 'MILK_TYPE',
    0x26: 'ANIMAL_MOTHER_NAME',
    0x27: 'UNUSED_27',
    0x28: 'RACE_WINNER_1',
    0x29: 'RACE_WINNER_2',
    0x2A: 'RACER_1_NAME',
    0x2B: 'RACER_2_NAME',
    0x2C: 'RACER_3_NAME',
    0x2D: 'RACER_4_NAME',
    0x2E: 'RACER_5_NAME',
    0x2F: 'RACER_6_NAME',
    0x30: 'FLOWER_CARD_POINTS',
    0x31: 'BAKERY_CARD_POINTS',
    0x32: 'AFFECTION_DISPLAY_1',
    0x33: 'AFFECTION_DISPLAY_2',
    0x34: 'AFFECTION_DISPLAY_3',
    0x35: 'AFFECTION_DISPLAY_4',
    0x36: 'AFFECTION_DISPLAY_5',
    0x37: 'AFFECTION_DISPLAY_6',
    0x38: 'AFFECTION_DISPLAY_7',
    0x39: 'AFFECTION_DISPLAY_8',
    0x3A: 'AFFECTION_DISPLAY_9',
    0x3B: 'AFFECTION_DISPLAY_10',
    0x3C: 'AFFECTION_DISPLAY_11',
}

GAMEVAR_MACRO_TO_INDEX = {v: k for k, v in GAMEVAR_MACROS.items()}

CHARACTER_AVATAR_MACROS: Dict[int, str] = {
    0: 'MARIA_1', 1: 'MARIA_2', 2: 'MARIA_3', 3: 'MARIA_4', 4: 'MARIA_5',
    5: 'POPURI_1', 6: 'POPURI_2', 7: 'POPURI_3', 8: 'POPURI_4', 9: 'POPURI_5',
    10: 'ELLI_1', 11: 'ELLI_2', 12: 'ELLI_3', 13: 'ELLI_4', 14: 'ELLI_5',
    15: 'ANN_1', 16: 'ANN_2', 17: 'ANN_3', 18: 'ANN_4', 19: 'ANN_5',
    20: 'KAREN_1', 21: 'KAREN_2', 22: 'KAREN_3', 23: 'KAREN_4', 24: 'KAREN_5',
    25: 'GRAY_1', 26: 'GRAY_2',
    27: 'CLIFF_1', 28: 'CLIFF_2', 29: 'CLIFF_3',
    30: 'JEFF_1', 31: 'JEFF_2', 32: 'JEFF_3',
    33: 'KAI_1', 34: 'KAI_2', 35: 'KAI_3',
    36: 'HARRIS_1', 37: 'HARRIS_2', 38: 'HARRIS_3',
    39: 'PASTOR_1', 40: 'PASTOR_2', 41: 'PASTOR_3',
    42: 'KENT_1', 43: 'KENT_2', 44: 'KENT_3',
    45: 'STU_1', 46: 'STU_2', 47: 'STU_3',
    48: 'MAY_1', 49: 'MAY_2', 50: 'MAY_3',
    51: 'MAYOR_1', 52: 'MAYOR_2',
    53: 'RICK_1', 54: 'RICK_2', 55: 'RICK_3',
    56: 'POTION_SHOP_DEALER_1', 57: 'POTION_SHOP_DEALER_2', 58: 'POTION_SHOP_DEALER_3',
    59: 'ELLEN_1', 60: 'ELLEN_2',
    61: 'MIDWIFE_1', 62: 'MIDWIFE_2',
    63: 'HARVEST_GODDESS_1', 64: 'HARVEST_GODDESS_2',
    65: 'GREG_1', 66: 'GREG_2', 67: 'GREG_3',
    68: 'DUKE_1', 69: 'DUKE_2',
    70: 'DOUG_1', 71: 'DOUG_2',
    72: 'SHIPPER_1', 73: 'SHIPPER_2',
    74: 'KAREN_6', 75: 'KAREN_7',
    76: 'ASSISTANT_CARPENTER_1', 77: 'ASSISTANT_CARPENTER_2',
    78: 'MASTER_CARPENTER_1', 79: 'MASTER_CARPENTER_2',
    80: 'HARVEST_SPRITE_1', 81: 'HARVEST_SPRITE_2',
    82: 'SAIBARA_1', 83: 'SAIBARA_2',
    84: 'BASIL_1', 85: 'BASIL_2',
    86: 'SASHA_1', 87: 'SASHA_2',
    88: 'LILLIA_1', 89: 'LILLIA_2',
    90: 'MAYOR_WIFE_1', 91: 'MAYOR_WIFE_2',
    92: 'MRS_MANA_1', 93: 'MRS_MANA_2',
    94: 'GOTZ_1', 95: 'GOTZ_2',
    96: 'ENTOMOLOGIST_1', 97: 'ENTOMOLOGIST_2',
    98: 'KAPPA_1',
    99: 'PLAYER_ENGRAVING_LOGO_1',
    100: 'BABY_1', 101: 'BABY_2', 102: 'BABY_3', 103: 'BABY_4', 104: 'BABY_5',
    105: 'OLD_MAN_1',
    106: 'OLD_WOMAN_1',
    107: 'DAD_1', 108: 'DAD_2',
    109: 'FESTIVAL_GIRL_1', 110: 'FESTIVAL_GIRL_2', 111: 'FESTIVAL_GIRL_3',
    112: 'JOHN_1', 113: 'JOHN_2', 114: 'JOHN_3',
    115: 'BARLEY_1',
    116: 'SHADY_SALESMAN_1', 117: 'SHADY_SALESMAN_2',
    118: 'PLAYER_ENGRAVING_LOGO_2', 119: 'PLAYER_ENGRAVING_LOGO_3', 120: 'PLAYER_ENGRAVING_LOGO_4',
    121: 'TOURIST_COUPLE_MAN_1', 122: 'TOURIST_COUPLE_WOMAN_1',
    123: 'PHOTOGRAPHER_1',
    124: 'GOURMET_JUDGE_1',
    125: 'SYDNEY_1',
    126: 'MARIA_6', 127: 'MARIA_7', 128: 'MARIA_8', 129: 'MARIA_9',
    130: 'ELLI_6', 131: 'ANN_6',
    132: 'KAREN_8', 133: 'KAREN_9',
    134: 'ELLI_7', 135: 'ELLI_8',
    136: 'MARIA_10', 137: 'POPURI_6', 138: 'KAREN_10', 139: 'ANN_7',
    140: 'GRAY_3', 141: 'JEFF_4', 142: 'CLIFF_4',
}

CHARACTER_AVATAR_MACRO_TO_INDEX = {v: k for k, v in CHARACTER_AVATAR_MACROS.items()}

# Bit masks for control byte (determines 1-byte vs 2-byte encoding per character)
BIT_MASKS = [0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01]

# Reverse mapping: character -> byte value
REVERSE_CHAR_MAP = {v: k for k, v in CHAR_MAP.items()}

def decode_char(byte_val: int) -> str:
    """Decode a single byte to its character representation."""
    return CHAR_MAP.get(byte_val, f'[0x{byte_val:02X}]')


def encode_char(char: str) -> int:
    """Encode a character to its byte value."""
    return REVERSE_CHAR_MAP.get(char, -1)
