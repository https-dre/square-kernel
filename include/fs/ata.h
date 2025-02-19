#define BASE_PORT 0x1F0
#define SECTOR_SIZE 512

extern int dev_write_word();
extern int dev_write();
extern int dev_read();

void wait_drive_until_ready();

void *read_disk(int);
void write_disk(int, short *);

void *read_disk_chs(int);
void write_disk_chs(int, short *);
