#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>  
#include <unistd.h>
/* write(), read(), close() fonksiyonlarını içeren kutuphanemiz*/ 
 
void func(int sockfd) 
{ 
	//Haberlesmede kullanilacak metni 80 karakter ile sinirladik. 80 karakteri gecen metinlerin ilk 80 karakterini gonderir
    char metin[80]; 
    int n; 
    //Bu for dongusu, client konusmayi bitirmek icin exit yazana kadar doner. Sonsuza kadar donmesi planlanir. 
    for (;;) { 
        bzero(metin, sizeof(metin)); 
        printf("Mesajınızı giriniz : "); 
        n = 0; 
        while ((metin[n++] = getchar()) != '\n') 
            ; 
        write(sockfd, metin, sizeof(metin)); 
        bzero(metin, sizeof(metin)); 
        read(sockfd, metin, sizeof(metin)); 
        printf("Sunucunun mesajı : %s", metin); 
        if ((strncmp(metin, "exit", 4)) == 0) { 
            printf("İstemciden cikildi...\n"); 
            break; 
        } 
    } 
} 
  
int main() 
{ 
    int sockfd; 
    struct sockaddr_in SunucuBilgileri; 
  
    // soket oluşturma ve doğrulama
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("Soket olusturmada hata olustu...\n"); 
        exit(0); 
    } 
    else
        printf("Soket basarili bir sekilde olusturuldu..\n"); 
    bzero(&SunucuBilgileri, sizeof(SunucuBilgileri)); 
  
    // IP, PORT atama
    SunucuBilgileri.sin_family = AF_INET;     
    SunucuBilgileri.sin_addr.s_addr  = htonl(INADDR_ANY); 
    SunucuBilgileri.sin_port = htons(5555); 
  
    // istemci soketini sunucu soketine bağlayın
    if (connect(sockfd, (struct sockaddr*)&SunucuBilgileri, sizeof(SunucuBilgileri)) != 0) { 
        printf("Servera baglanirken hata olustu...\n"); 
        exit(0); 
    } 
    else
        printf("Servere baglanti saglandi..\n"); 
  
    // sohbet işlevi
    func(sockfd); 
  
    // soketi kapat
    close(sockfd); 
} 
