; Gabinete, Keith Ginoel S.
; CMSC 125 - CD 7L
; October 05, 2023

; This is the boot sector code

; --------------------------------------------------------------------------------------------------------------------
; Start matter
; --------------------------------------------------------------------------------------------------------------------
[BITS 16]			; Tells the compiler to make this into 16-bit code generation
[ORG 0x7C00]			; Tells the compiler where the code is going to be
				; in memory after it has been loaded. (HEX number)

start:

	mov [bootdrv], dl       	; Before anything else, take note of the 'drive number' where we booted from
					; DL tells us what 'drive number' we booted from, we need this later
					; Store the 'drive number' to the variable 'bootdrv'

	; Setup the stack to be used in function calls.
	; We can't allow interrupts while we set it up

	cli                     ; Disable interrupts (CLear Interrupts bit)
	mov ax, 0x9000          ; Put stack at 9000:0000
	mov ss, ax              ;
	mov sp, 0               ;
	sti                     ; Enable interrupts (SeT Interrupts bit)

; --------------------------------------------------------------------------------------------------------------------
; This is the main loop for the boot loader. It will wait for the "boot" command to be
; entered before the kernel is read from second sector
; see 'routines.asm' for the functions or procedures
; --------------------------------------------------------------------------------------------------------------------
mainloop:
				; Display the "grub>" prompt
				; Instructions:
				; mov SI, grub
				; call the putstr

	mov si, prompt
	call putstr
				

				; Begin accepting "commands" or input
				; Instructions:
				; call the getstr function
				; before calling the getstr function, put buffer to DI
	mov di, buffer
	call getstr
				

				; If no command is entered show the prompt again
				; jump to main loop
	mov si, buffer
	cmp byte [si], 0
	je mainloop
				

				; compare the command issued to the "boot" command
				; Instructions:
				; call the strcmp function
				; cmd = si or di
				; si = di
				; jc, kung saan gusto pumunta (load kernel)
				; call .load_kernel, prompt a message after

				; we jump to the routine which loads the kernel when the "boot" command is issued
	mov si, buffer
	mov di, cmd_boot
	call strcmp
	jc .load_kernel

				; otherwise we just show the prompt again
	mov si, prompt
	call putstr
	jmp mainloop

	.load_kernel:
					; we show some message telling that the kernel is being loaded



	call read_kernel        	; Load stuff from the bootdrive

	jmp dword KERNEL_SEGMENT:0	; we jump now to the memory location where the kernel was loaded

; --------------------------------------------------------------------------------------------------------------------
; Data section
; Functions and variables used by our bootstrap
; --------------------------------------------------------------------------------------------------------------------
prompt  db "kgsgabinete@cmsc125cd7l>",0 ; palitan initials at section
msg     db "Loading kernel...",13,10,0
cmd_boot  db "boot",0
buffer times 64 db 0			; an empty string
bootdrv db 0                    	; The boot drive id
KERNEL_SEGMENT equ 0x1000

; --------------------------------------------------------------------------------------------------------------------
; Read few sectors from the BOOT DRIVE
; --------------------------------------------------------------------------------------------------------------------
read_kernel:
	push ds                 ; save ds
	.reset:
	mov ax, 0               ; Reset Disk first before read
	mov dl, [bootdrv]       ; Drive to reset
	int 13h                 ;
	jc .reset               ; Failed -> Try again

	pop ds

 .read:
				; insert lines of code for reading the kernel sector
				; you will need to use INT 0x13 (see your handouts)
				; carefully, initialize the necessary registers (see your handouts)

	; invoke the BIOS interrupt for reading disk sectors
	; modify the following registers

	; ah = 0x02
	; al = 1
	; ch = 0
	; cl = 2
	; dh = 0
	; dl = 0
	; ilagay kay es si kernel segment, bx naman ay 0
	; mov  , KS
	; mov es, _____
	; mov bx, 0
	; call bios interrupt= int 0x13

	mov ah, 0x02
	mov al, 1
	mov ch, 0
	mov cl, 2
	mov dh, 0
	mov dl, 0

	mov bx, KERNEL_SEGMENT
	mov es, bx
	mov bx, 0

	int 0x13


  retn

; Includes "routines.asm"
	%include "routines.asm"

; --------------------------------------------------------------------------------------------------------------------
; End matter
; --------------------------------------------------------------------------------------------------------------------
	times 510-($-$$) db 0	; Fill the rest of the sector with zero's
	dw 0xAA55		; Add the boot loader signature at the end, size is 2 bytes
