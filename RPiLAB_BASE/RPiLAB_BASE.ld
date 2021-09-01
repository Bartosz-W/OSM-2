
MEMORY
{
	ram : ORIGIN = 0x100000, LENGTH = 0x200000

}

SECTIONS
{
	. = 0x100000;
	.begin
	 : {
		*(.begin)
	} > ram
	
	.text : {
		*(.text*)
	} > ram

	_etext = .;

	.rodata : {
		*(.rodata*)
	} > ram

	.data : {
		*(.data*)
	} > ram

	__bss_start = .;

	.bss : {
		*(.bss*)
	} > ram

	__exidx_start = .;
	.exidx : {
		*(.exidx*)
	} > ram
	__exidx_end = .;
	_end = .;
	
		end = .;
	


}
