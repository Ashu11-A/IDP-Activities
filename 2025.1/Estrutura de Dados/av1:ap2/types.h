typedef struct {
  float value;
  int months;
  float plot;
  int approved;
} Loan;

typedef struct {
  char name[128];
  float salary;
  Loan loan;
} Client;

void clearCache();
void clearScreen();
int getInt();
int validateInt(const char *str);
int menu(Client *client);
int selectClient(Client *clients, int numClients);