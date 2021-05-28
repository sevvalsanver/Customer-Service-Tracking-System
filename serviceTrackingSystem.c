#include <stdio.h>

#include <stdlib.h>

#define MAX_SIZE 11

/*
Daha önce geleni daha erken isleme alabilmek icin queue yapisini kullandim. Kuyruklarin özellikleri farkli oldugu icin 3 adet queue olusturdum.
Tek bir queue yapisi üzerinden islemleri gerceklestirebilmek icin struct yapisini kullandim. Kuyruga veri eklemek için enqueue, kuyruktan veri cikarmak
icin dequeue yapisini kullandim. Komutlari bir dosyaya yazdirip ordan okumamak için logs arrayinde tuttum. Olusturulan queue icin hafizada yer olusturmak amacli malloc(sizeof())
yapisini kullandim. Dosyadan okuma icin fprintf ve dosyaya yazma icin fscanf fonksiyonlarini kullandim. Dosyadaki verileri istenilen degiskenle karsilastirmak icin strcmp
komutunu kullandim. Stringleri belirli bir formatta log degiskenine yazdirmak için sprintf komutunu kullandim. Strcpy komutunu bir stringi log arrayine kopyalamak için kullandim.
*/

// queue nun genel degiskenlerini kullanmak için kullandigimiz struct
typedef struct queue {
  char items[MAX_SIZE][128]; // customer isimlerini tutmak icin 10(MAX_SIZE) adet 128 byte lik string
  int size; // queue daki item sayisi
  int front; // en ondeki elemanin indexi
  int rear; // en arkadaki elemanin indexi
}
queue;




char logs[100][100]; // ServeCustomer komutunun ciktilarini tutan dizi
int logIndex = 0; // kaç adet komut ciktisi oldugunu tutmak icin kullaniyoruz

void dequeue(queue * q, char type); //queue dan eleman cikarma
void newCustomer(queue * q, char * name); //queue ya eleman ekleme
void serveCustomer(queue * q, char type, int numberOfCustomers);  //ServeCustomer komutu gelince numberofCustomers degiskeni kadar dequeue komutunu cagirma

// queue olusturma
queue * createQueue() {
  queue * q = (queue * ) malloc(sizeof(queue)); // hafizada queue struct inin boyutu kadar yer acar
  // queue bos oldugunda front ve rear degerleri -1 dir
  q -> front = -1;
  q -> rear = -1;
  // queue da eleman olmadigi icin size=0
  q -> size = 0;
  return q;
}


void dequeue(queue * q, char type) {

  char log[128]; // ciktiyi tutacagimiz string

  if (q -> size == 0) { // queue bossa name yerine ***** döndür
    sprintf(log, "%c\t*****", type); // log a yazdýr
  } else {
    // eðer boþ deðilse sýradaki item(q -> items[q -> front]) name i alarak loga type ile birlikte yazdiriyoruz
    sprintf(log, "%c\t%s", type, q -> items[q -> front]);

    // front indexini 1 arttirip sonraki iteme getiriyoruz queue dan 1 eleman eksildigi için size i 1 azaltiyoruz
    q -> front++;
    q -> size--;
    // queue bossa front,rear=-1
    if (q -> size == 0){
        q -> rear = -1;
        q -> front = -1;

    }

  }

  strcpy(logs[logIndex], log); // kayitlari, tuttugumuz log array ine kopyalıyoruz
  logIndex++;  // log sayisini arttiriyoruz ki bir sonraki kayit arrayin sonraki elemanina eklensin
}


//enqueue
void newCustomer(queue * q, char * name) {
  if (q -> rear == MAX_SIZE - 1) // rear indeximiz son elemandaysa queue daha fazla eleman alamaz
    return; // queue full
  else { // queue dolu degilse
    if (q -> front == -1) // eger eklenen ilk item ise front -1 den 0
      q -> front = 0;



    q -> rear++; // rear indexini arttirarark arrayin sonraki bos elemanina getirelim
    q -> size++; // yeni bir item eklendigi için size 1 arttiralim
    strcpy(q -> items[q -> rear], name); // yeni elemani arrayimizin sonraki rear indexine kopyalayalim
  }
}
void serveCustomer(queue * q, char type, int numberOfCustomers) {
  int i = 0;
  // numberOfCustomers kadar dequeue cagir

  for (; i < numberOfCustomers; i++) {
    dequeue(q, type);
  }

}

int main() {

 // her bir grup icin birer queue olustur
  queue * adultQueue = createQueue();
  queue * childQueue = createQueue();
  queue * elderQueue = createQueue();

  FILE * f = fopen("input.txt", "r"); // input.txt dosyasini okumak icin ac
  if (!f) {
    printf("Failed to load file\n"); // dosya acma basarili degilse programi sonlandir
    return 0;
  }

  while (!feof(f)) { // dosyanin sonuna gelene kadar dön
    char param0[64], param1, param2[10];


    fscanf(f, "%s\t%c\t%s ", param0, & param1, param2); // input dosyasindan veri okuma tablarla ayrilmis sekilde

    if (!strcmp(param0, "NewCustomer")) { // eger param0 NewCustomer ise
            /*
            param1 yani kullanici type ine ait queue yu newCustomer fonksiyonuna param2(kullanici adi) ile birlikte gönderiyoruz
            */

      if (param1 == 'E') {
        newCustomer(elderQueue,  param2);
      } else if (param1 == 'A') {
        newCustomer(adultQueue,  param2);
      } else if (param1 == 'C') {
        newCustomer(childQueue,  param2);
      }

    } else if (!strcmp(param0, "ServeCustomers")) {
      /*
            param1 yani kullanici type ine ait queue yu serveCustomer fonksiyonuna param2(kac adet kullanici alinacagi bilgisi ile) ile birlikte(int e donusturup) gonderiyoruz
            */


      if (param1 == 'E') {
        serveCustomer(elderQueue, param1, atoi(param2));
      } else if (param1 == 'A') {
        serveCustomer(adultQueue, param1, atoi(param2));
      } else if (param1 == 'C') {
        serveCustomer(childQueue, param1, atoi(param2)); // atoi char degeri integer e donusturur
      }
    }

  }
    // output.txt dosyasini yazmak icin ac
  FILE * o = fopen("output.txt", "w");
  // log dizisinde tuttugumuz verileri sondan basa yazdiriyoruz
  int k = 0;
  if (logIndex > 0) {
    for (k = logIndex - 1; k >= 0; k--) {
      fprintf(o, "%s\n", logs[k]);
    }
  }


// dosyalari kapatiyoruz
  fclose(f);
  fclose(o);

  return 0;
}
