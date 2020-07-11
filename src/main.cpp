#include <Arduino.h>

#include <JFAT.h> 

JFAT file_sys; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  delay(2000); 
  int status = file_sys.begin(PA2); 
  if(status < 0){
    Serial.println("File Sys init failed!"); 
    while(true); 
  }

  // try to create files and check stability 
  file_sys.delete_all_files(); 
  uint8_t write_buff[513]; 
  for(int i = 0; i < 513; i ++){
    write_buff[i] = 42; 
  }
  write_buff[512] = 200; 
  unsigned start = micros(); 
  file_sys.open_file_write(); 
  for(int i = 0; i < 10; i ++){
    file_sys.write(write_buff, 513); 
  }
  file_sys.close(); 
  unsigned long end = micros(); 
  Serial.println("Write Time: " + String(end - start)); 

  JFAT::FileList list; 
  file_sys.read_fat(&list); 
  file_sys.open_file_read(list.num_files-1); 
  unsigned int len = file_sys.peek(); 
  Serial.println("Size Avail: " + String(len)); 
  uint8_t read_buf[len]; // not suggested for normal use, this can easily fill device storage 
  file_sys.read(read_buf, len); 
  Serial.println("\n\nRecovered data: "); 
  for(int i = 0; i < len; i ++){
    Serial.print("\t");
    Serial.print(read_buf[i]); 
    if((i + 1) % 16 == 0) Serial.println();  
  }
  file_sys.close(); 
  // open and close a bogus file real quick 
  file_sys.open_file_write(); 
  file_sys.close(); 
  file_sys.read_fat(&list); 
  file_sys.open_file_read(list.num_files-1); 
  Serial.println("\nSize Bogus File: " + String(file_sys.peek()));



}

void loop() {
  // put your main code here, to run repeatedly:
}