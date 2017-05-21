
#define BTN_A 0xE0E036C9
#define BTN_B 0xE0E028D7
#define BTN_C 0xE0E0A857
#define BTN_D 0xE0E06897

typedef struct _bind {
  unsigned long button;
  char* command;
} CommandBind;
