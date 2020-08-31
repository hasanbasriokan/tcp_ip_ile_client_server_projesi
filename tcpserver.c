#include <stdio.h> 
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h> 
/*
struct sockaddr_in {
    short            sin_family;   
    unsigned short   sin_port;     
    struct in_addr   sin_addr;     
    char             sin_zero[8];  
};

struct in_addr {
    unsigned long s_addr;  // load with inet_aton()
};
*/

#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
  
 
//İstemci ve sunucu arasında sohbet için tasarlanmış işlev.
void func(int sockfd) 
{ 
    char metin[80]; 
    int n; 

    // sohbet için sonsuz döngü
    for (;;) { 
        bzero(metin, 80); 
  
        // mesajı istemciden okuyun ve arabelleğe kopyalayın
        read(sockfd, metin, sizeof(metin)); 


        // İstemci içeriğini içeren yazdırma arabelleği
        printf("İstemciden gelen: %s\t İstemciye gonder : ", metin); 
        bzero(metin, 80); 
        n = 0; 

        // arabellekte sunucu mesajını kopyala
        while ((metin[n++] = getchar()) != '\n') 
            ; 
  
        //ve bu metni istemciye gönder
        write(sockfd, metin, sizeof(metin)); 
  
        // Mesaj "exit" içeriyorsa sunucudan çıkılır ve sohbet sona erer.
        if (strncmp("exit", metin, 4) == 0) { 
            printf("Sunucudan cikildi...\n"); 
            break; 
        } 
    } 
} 
  
// Sürücü işlevi
int main() 
{ 
    int sockfd, connfd ,uzunluk; 
    struct sockaddr_in SunucuBilgileri;
    struct sockaddr_in cli; 
  
    // soket oluşturma ve doğrulama
    // SOCK STREAM ifadesi Tcp protokolunu kullanmak icindir. Eger ki Udp protokolu ile islem yapmak istersek, SOCK_DGRAM ifadesini kullanmamız gerekir.
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("Soket olusturulamadı...\n"); 
        exit(0); 
    } 
    else
        printf("Soket olusturuldu... \n"); 
    bzero(&SunucuBilgileri, sizeof(SunucuBilgileri)); 
  
    // IP, PORT atama
	// Ipv4 ifadesine karşılık gelen ifade AF_INET. Ipv6 ifadesine karsilik gelen ifade ise AF_INET6 koyarsak 
    SunucuBilgileri.sin_family = AF_INET; 
    SunucuBilgileri.sin_addr.s_addr  = htonl(INADDR_ANY); 
    SunucuBilgileri.sin_port = htons(4444); 
  
    // Yeni oluşturulan soketi verilen IP'ye bağlama ve doğrulama
    if ((bind(sockfd, (struct sockaddr *)&SunucuBilgileri, sizeof(SunucuBilgileri))) != 0) { 
        printf("Soket, ip adresine baglanamadı...\n"); 
        exit(0); 
    } 
    else
        printf("Soket, ip adresine baglandı..\n"); 
  
    // Artık sunucu dinlemeye ve doğrulamaya hazır
    if ((listen(sockfd, 5)) != 0) { 
        printf("Sunucuyu dinleme işlemi başarısız oldu...\n"); 
        exit(0); 
    } 
    else
        printf("Sunucudan mesaj bekleniyor..\n"); 
		uzunluk = sizeof(cli); 
		// İstemciden gelen veri paketini kabul edin ve doğrulayın
		connfd = accept(sockfd, (struct sockaddr *)&cli, &uzunluk); 
    if (connfd < 0) { 
        printf("Sunucu ile baglanti basarisiz oldu...\n"); 
        exit(0); 
    } 
    else
        printf("Sunucu, istemci ile baglanti sagladi...\n"); 
  
    // İstemci ve sunucu arasında sohbet etme işlevi
    func(connfd); 
  
    // Sohbet ettikten sonra soketi kapat
    close(sockfd); 
} 
