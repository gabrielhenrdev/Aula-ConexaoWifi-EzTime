
#include <Arduino.h>
#include <WiFi.h>
#include <ezTime.h>

// Protótipo das funções
void conexaoWIFI();
void reconexaoWIFI();
void exibirData();

// WiFi
const char *SSID = "SENAI EDUCACIONAL";
const char *SENHA = "info@134";

const unsigned long tempoConexao = 10000;
const unsigned long tempoReconexao = 5000;

int minuto;
int segundo;
int hora;
int hora12;
int dia;
int diaAno;
int mes;
int ano;

bool am;
bool pm;

Timezone sp;

// reconexão
unsigned long tempoultimaConexao = 0;

const char *meses[] = {
    "Janeiro", "Fevereiro", "Março", "Abril",
    "Maio", "Junho", "Julho", "Agosto",
    "Setembro", "Outubro", "Novembro", "Dezembro"
};

const char *diasSemana[] = {
    "domingo",
    "segunda-feira",
    "terça-feira",
    "quarta-feira",
    "quinta-feira",
    "sexta-feira",
    "sábado"
};

void setup() {

    Serial.begin(115200);

    //  WiFi
    conexaoWIFI();

    sp.setLocation("America/Sao_Paulo");

    if (WiFi.status() == WL_CONNECTED) {

        waitForSync();

        minuto = sp.minute();
        segundo = sp.second();
        hora = sp.hour();
        hora12 = sp.hourFormat12();
        dia = sp.day();
        diaAno = sp.dayOfYear();
        mes = sp.month();
        ano = sp.year();

        am = sp.isAM();
        pm = sp.isPM();

        exibirData();
    }
}

void loop() {

    reconexaoWIFI();

}

void conexaoWIFI() {

    Serial.printf("Conectando ao WiFi: %s\n", SSID);

    WiFi.begin(SSID, SENHA);

    unsigned long tempoWifi = millis();

    while (WiFi.status() != WL_CONNECTED &&
           millis() - tempoWifi < tempoConexao) {

        Serial.print(".");
        delay(500);
    }

    if (WiFi.status() == WL_CONNECTED) {

        Serial.println("\nConectado ao WiFi!");

        Serial.print("Endereco IP: ");
        Serial.println(WiFi.localIP());

    } else {

        Serial.println("\nFalha ao se conectar.");
    }
}

void reconexaoWIFI() {

    unsigned long tempoAtual = millis();

    if (tempoAtual - tempoultimaConexao >= tempoReconexao) {

        tempoultimaConexao = tempoAtual;

        if (WiFi.status() != WL_CONNECTED) {

            Serial.println("Voce perdeu a conexao!");

            conexaoWIFI();

            if (WiFi.status() == WL_CONNECTED) {

                waitForSync();
            }
        }
    }
}

void exibirData() {

    int diaSemana = sp.weekday();

    int diasRestantes = 365 - diaAno;

    Serial.print("Hoje e ");
    Serial.print(dia);
    Serial.print(" de ");
    Serial.print(meses[mes - 1]);
    Serial.print(" de ");
    Serial.print(ano);

    Serial.print(", ");
    Serial.print(diasSemana[diaSemana - 1]);

    Serial.print(" e faltam ");
    Serial.print(diasRestantes);
    Serial.println(" dias para o ano novo.");
}
