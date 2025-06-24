volatile char *out, *clk;
int sum[4];
void write_data(int d);
void start();
void stop();
void bits_one(int data);
void clock(int h);
void dig_array(int num);
int count(int d);
void tm_init() {
  volatile char *dirk, *dirf;
  dirk = 0x30;
  dirf = 0x107;
  *dirk = 0xff;
  *dirf = 0xff;
  out = 0x31;
  clk = 0x108;
}
void write_data(int d) {
  char digit1[10] = {
  0x3F,  // 0
  0x06,  // 1
  0x5B,  // 2
  0x4F,  // 3
  0x66,  // 4
  0x6D,  // 5
  0x7D,  // 6
  0x07,  // 7
  0x7F,  // 8
  0x6F   // 9
};
  dig_array(d);
  int r;
  start();
  bits_one(0x40);
  stop();
  delay(10);
  start();
  bits_one(0xC0);
  for(int i=3;i>=0;i--) {
 
    bits_one(digit1[sum[i]]);
  }
  stop();
  delay(10);
  start();
  bits_one(0x8F);
  stop();
}
void clock(int h) {
  if (h == 1) {
    *clk |= 0x01;
  }
  else {
    *clk &= ~0x01;
  }
}
void start() {
  *out |= 0x01;
  clock(1);
  *out &= ~0x01;
  delay(2);
}
void stop() {
  clock(0);
  *out &= ~0x01;
  clock(1);
  *out |= 0x01;
  delay(2);
}
void bits_one(int data) {
  for (int i = 0; i < 8; i++) {
    char b = (data >> i) & 1;
    clock(0);
    if (b == 1) {
      *out |= 0x01;
    }
    else {
      *out &= ~0x01;
    }
    delay(2);
    clock(1);
    delay(2);
  }
  // Dummy ACK after data_bits
  clock(0);
  *out |= 0x01;
  delay(2);
  clock(1);
  delay(2);
  clock(0);
}
void dig_array(int num) {
  int r;
  // Initialize all values to 0
  for(int i = 0; i < 4; i++) {
    sum[i] = 0;
  }

  for(int i = 0; i < 4 && num > 0; i++) {
    r = num % 10;
    num = num / 10;
    sum[i] = r;
  }
}

int count(int d){
  int c=0;
  while(d>0){
   c++; 
   d=d/10;
  }
  return c;
}
