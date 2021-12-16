%include "boot.inc"

section loader vstart=LOADER_BASE_ADDR
LOADER_STACK_TOP equ LOADER_BASE_ADDR

jmp loader_start

; 这里其实就是GDT的起始地址，第一个描述符为空
GDT_BASE: dd 0x00000000
          dd 0x00000000

; 代码段描述符，一个dd为4字节，段描述符为8字节，上面为低4字节
; 段界限全是f，所以能最大访问4G
CODE_DESC: dd 0x0000FFFF
           dd DESC_CODE_HIGH4

; 栈段描述符，和数据段共用
DATA_STACK_DESC: dd 0x0000FFFF
                 dd DESC_DATA_HIGH4

; 显卡段，非平坦
; 显存的地址范围是：0xb8000~0xbffff。b在高四个字节中哦，在头文件中已经设置过了。
; 低四个字节从b开始。按照粒度为4k划分，最大限度为7
VIDEO_DESC: dd 0x80000007
            dd DESC_VIDEO_HIGH4

GDT_SIZE equ $ - GDT_BASE
GDT_LIMIT equ GDT_SIZE - 1
times 120 dd 0
SELECTOR_CODE equ (0x0001 << 3) + TI_GDT + RPL0 ;分别是三个选择子
SELECTOR_DATA equ (0x0002 << 3) + TI_GDT + RPL0
SELECTOR_VIDEO equ (0x0003 << 3) + TI_GDT + RPL0

gdt_ptr dw GDT_LIMIT ;低两个字节表GDT界限
        dd GDT_BASE  ;高四个字节表GDT在内存的起始地址:0+start=0x900

loadermsg db '2 loader in real.'

loader_start: 

        ; 调用10h号中断显示字符串
        mov sp, LOADER_BASE_ADDR
        mov bp, loadermsg
        ; 字符串长度
        mov cx, 17
        ; 子功能号以及显示方式
        mov ax, 0x1301
        ; 页号:0, 蓝底粉红字
        mov bx, 0x001f
        mov dx, 0x1800
        int 0x10

        ; 打开A20地址线
        in al, 0x92
        or al, 00000010B
        out 0x92, al

        ; 加载gdt
        lgdt [gdt_ptr]

        ; cr0第0位置1
        mov eax, cr0
        or eax, 0x00000001
        mov cr0, eax

        ; 刷新流水线
        jmp dword SELECTOR_CODE:p_mode_start

[bits 32]
p_mode_start:
    mov ax, SELECTOR_DATA
    mov ds, ax

    mov es, ax
    mov ss, ax

    mov esp, LOADER_STACK_TOP
    mov ax, SELECTOR_VIDEO
    mov gs, ax

    mov byte [gs:160], 'P'
    ; 与书中不同，这里使用蓝底白字
    mov byte [gs:161], 0x1f

    jmp $

