void some_fun(){
  
}

void main() {
  char * video_memory = (char*) 0xb8000;
  *video_memory = 'X';
  some_fun();
}
