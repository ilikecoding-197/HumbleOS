char *vgaBuffer = (char *)0xb8000;

void kernel_main(){
	vgaBuffer[0] = 'H'; // Write a H to the screen to test C!
}
