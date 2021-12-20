;---------------------------loader和kernel-------------
LOADER_BASE_ADDR equ 0x900 ;loader放入内存的位置
LOADER_START_SECTOR equ 0x2  ;loader从硬盘读取loader的位置。即loader在硬盘的存放位置


; gdt描述符属性
; 段描述符高23位，表示段界限的粒度为4KB.段界限*粒度=2^20*4k=4G
DESC_G_4K equ 100000000000000000000000b   

; D/B为，1表示运行在32位模式下.(地址和操作数是32位)
DESC_D_32 equ 10000000000000000000000b

; 高21位，如果为1表示为64位代码段，目前我们都是在32位模式下操作，故为零
DESC_L equ     0000000000000000000000b

; 没有明确的用途，取值随意
DESC_AVL equ   000000000000000000000b

; 第二部分段界限值，由于采用了32位平坦模型，所以段界限为(4GB / 4KB) - 1 = 0xFFFFF，故为全1
DESC_LIMIT_CODE2 equ 11110000000000000000b
DESC_LIMIT_DATA2 equ DESC_LIMIT_CODE2

; 书中取全零，怀疑是错误的，因为保护模式的基地址是0xb8000，所以最后8位应该是b，而不是0
;书中应该是写错了
DESC_LIMIT_VIDEO2 equ 00000000000000000000000000001011b

DESC_P equ 1000000000000000b ;在内存中，P置1
DESC_DPL_0 equ 000000000000000b ;下面几个是特权级，分别是0，1，2，3
DESC_DPL_1 equ 010000000000000b
DESC_DPL_2 equ 100000000000000b
DESC_DPL_3 equ 110000000000000b

DESC_S_CODE equ  1000000000000b ;S为1是非系统段，S为0是系统段
DESC_S_DATA equ  DESC_S_CODE
DESC_S_sys equ  0000000000000b

;x=1,c=0,r=0,a=0 代码段可执行，非一致性，不可读，已访问位清零
DESC_TYPE_CODE equ 100000000000b
;x=0,c=0,r=1,a=0 数据段不可执行，可读写数据段，已访问位清零
DESC_TYPE_DATA equ 001000000000b

; 代码段描述符的高32位表示，其中(0x00 << 24表示最高8位的段基址值，由于我们采用的是平坦模型，故基址为零)，后面唯一可变的就是段界限值
DESC_CODE_HIGH4 equ (0x00 << 24) + DESC_G_4K + DESC_D_32 + \
    DESC_L + DESC_AVL + DESC_LIMIT_CODE2 + \
    DESC_P + DESC_DPL_0 + DESC_S_CODE + DESC_TYPE_CODE + 0x00

;数据段描述符高32位
DESC_DATA_HIGH4 equ (0x00 << 24) + DESC_G_4K + DESC_D_32 + \
    DESC_L + DESC_AVL + DESC_LIMIT_DATA2 + \
    DESC_P + DESC_DPL_0 + DESC_S_DATA + DESC_TYPE_DATA + 0x00

;显存段描述符高32位
DESC_VIDEO_HIGH4 equ (0x00 << 24) + DESC_G_4K + DESC_D_32 + \
    DESC_L + DESC_AVL + DESC_LIMIT_VIDEO2 + \
    DESC_P + DESC_DPL_0 + DESC_S_DATA + DESC_TYPE_DATA + 0x00

; 选择字属性
RPL0 equ 00b;低两位的请求特权
RPL1 equ 01b
RPL2 equ 10b
RPL3 equ 11b
TI_GDT equ 000b;0代表GDT，1代表LDT
TI_LDT equ 100b


;------------页目录属性----------
;页目录地址
PAGE_DIR_TABLE_POS equ 0x100000

;-----------------页表属性-------------
PG_P equ 1b
PG_RW_R  equ 00b
PG_RW_W  equ 10b
PG_US_S  equ 000b
PG_US_U  equ 100b
