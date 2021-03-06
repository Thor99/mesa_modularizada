#include <SoftwareSerial.h>
#include <ShiftRegister74HC595.h>
#include <Motor595.h>
#include <HashMap.h>
#include <Servo.h>

Motor595 sr (10,11,12);

Servo servo;
 
const byte HASH_SIZE = 8;

HashType<char,int> hashRawArray[HASH_SIZE];
HashMap<char,int> hashMap = HashMap<char,int>(hashRawArray, HASH_SIZE);

String command;
int umaCasa = 62


; // Passos para andar uma casa
int meiaCasa = umaCasa / 2; // Passos para andar meia casa
int numeroDeJogadas = 0;
int posCemiterioY[] = {meiaCasa, meiaCasa + umaCasa, meiaCasa + 2 * umaCasa, meiaCasa + 3 * umaCasa, meiaCasa + 4 * umaCasa, meiaCasa + 5 * umaCasa, meiaCasa + 6 * umaCasa, meiaCasa + 7 * umaCasa};
int posCemiterioX[] = {meiaCasa, meiaCasa + 8 * umaCasa, meiaCasa + umaCasa, meiaCasa + 9 * umaCasa};  
int posTabuleiro[] = {meiaCasa, meiaCasa + umaCasa, meiaCasa + 2 * umaCasa, meiaCasa + 3 * umaCasa, meiaCasa + 4 * umaCasa, meiaCasa + 5 * umaCasa, meiaCasa + 6 * umaCasa, meiaCasa + 7 * umaCasa};
int cemiterioJ1 = 0;
int cemiterioJ2 = 0;
int eletroIma = 9;
int botaoX = 5;
int botaoY = 6;

void setup(){
    Serial.begin(9600);
    pinMode(botaoX, INPUT);
    pinMode(botaoY, INPUT);
    servo.attach(9);
    servo.write(0);

    sr.setSpeed(35);
    
    hashMap[0]('A',0);
    hashMap[1]('B',1);
    hashMap[2]('C',2);
    hashMap[3]('D',3);
    hashMap[4]('E',4);
    hashMap[5]('F',5);
    hashMap[6]('G',6);
    hashMap[7]('H',7);
    // Serial.println(hashMap.getValueOf("A")) retornaria 0
}

void loop(){
    
    if(Serial.available()){
            command = Serial.readString();
        

        Serial.println(command);

        /*
        Frase: "*posiçãoinicial tipodejogada posiçãofinal#"
        Posição inicial: tamanho 2
        Posição final: tamanho 2
        Tipo de jogada pode ser "atacar" ou "para"
        */

        if(command.substring(3, 9) == "atacar" || command.substring(3, 9) == "ATACAR"){ // Jogada de ataque
            // Chamar função de ataque
            Serial.println(command.substring(0, 2));
            Serial.println(command.substring(10, 12));
            atacarPeca(command.substring(0, 2), command.substring(10, 12));
            Serial.println("fim");
        }

        else if(command.substring(3, 7) == "para" || command.substring(3, 7) == "PARA"){ // Jogada de movimento (para)
            // Chamar função de movimento
            Serial.println(command.substring(0, 2));
            Serial.println(command.substring(8, 10));
            moverPeca(command.substring(0, 2), command.substring(8, 10));
            Serial.println("fim");
        }

        else {
            // Movimento hockey
        }

        command = "";
    }
}

void moverPeca(String posicaoInicial, String posicaoFinal){
    Serial.println("Certo 1");
    sr.step(posTabuleiro[hashMap.getValueOf(posicaoInicial[0])], 1, 0);
    Serial.println("Pos step 1");
    delay(50);
    Serial.println(posTabuleiro[(int) posicaoInicial[1] -48 - 1]);
    sr.step(posTabuleiro[(int) posicaoInicial[1] -48 - 1], 1, 1);
    Serial.println("Pos step 2");
    delay(50);   
    servo.write(73);
    delay(1000);
    Serial.println("Pos Servo");
    if(posicaoFinal[0] > posicaoInicial[0]){
      if((int) posicaoFinal[1] - 48 > (int) posicaoInicial[1] - 48){
        Serial.println("Certo 2");
        sr.step(meiaCasa, 1, 0);
        delay(50);
         sr.step(posTabuleiro[(int) posicaoFinal[1] - 48 - 1] - posTabuleiro[(int) posicaoInicial[1] - 48 - 1] - meiaCasa, 1, 1);
         delay(50);
         sr.step(posTabuleiro[hashMap.getValueOf(posicaoFinal[0])] - posTabuleiro[hashMap.getValueOf(posicaoInicial[0])] - meiaCasa, 1, 0);
         delay(50);
         sr.step(meiaCasa, 1, 1);
         delay(50);
      }
      else if((int) posicaoFinal[1] - 48 < (int) posicaoInicial[1] - 48){
        sr.step(meiaCasa, 1, 0);
        delay(50);
        sr.step(posTabuleiro[(int) posicaoInicial[1] - 48 - 1] - posTabuleiro[(int) posicaoFinal[1] - 48 - 1] - meiaCasa, 0, 1);
        delay(50);
        sr.step(posTabuleiro[hashMap.getValueOf(posicaoFinal[0])] - posTabuleiro[posicaoInicial[hashMap.getValueOf(posicaoInicial[0])]] - meiaCasa, 1, 0);
        delay(50);
        sr.step(meiaCasa, 0, 1);
        delay(50);
      }
      else{
        sr.step(meiaCasa, 1, 1);
        delay(50);
        sr.step(posTabuleiro[hashMap.getValueOf(posicaoFinal[0])] - posTabuleiro[hashMap.getValueOf(posicaoInicial[0])], 1, 0);
        delay(50);
        sr.step(meiaCasa, 0, 1);
        delay(50);
      }
      
    }
    
    else if(posicaoFinal[0] < posicaoInicial[0]){
           
      if((int) posicaoFinal[1] - 48 > (int) posicaoInicial[1] - 48){
        sr.step(meiaCasa, 0, 0);
        delay(50);
        sr.step(posTabuleiro[(int) posicaoFinal[1] - 48 - 1] - posTabuleiro[(int) posicaoInicial[1] - 48 - 1] - meiaCasa, 1, 1);

        delay(50);
        sr.step(posTabuleiro[hashMap.getValueOf(posicaoInicial[0])] - posTabuleiro[hashMap.getValueOf(posicaoFinal[0])] - meiaCasa, 0, 0);
        delay(50);
        sr.step(meiaCasa, 1, 1);
        delay(50);
      }
      else if((int) posicaoFinal[1] - 48 < (int) posicaoInicial[1] - 48){
       sr.step(meiaCasa, 0, 0);
       delay(50);
       sr.step(posTabuleiro[(int) posicaoInicial[1] - 48 - 1] - posTabuleiro[(int) posicaoFinal[1] - 48 - 1] - meiaCasa, 0, 1);
       delay(50);
       sr.step(posTabuleiro[hashMap.getValueOf(posicaoInicial[0])] - posTabuleiro[hashMap.getValueOf(posicaoFinal[0])] - meiaCasa, 0, 0);
       delay(50);
       sr.step(meiaCasa, 0, 1);
       delay(50);
      }
      else{
        sr.step(meiaCasa,1, 1);
        delay(50);
        sr.step(posTabuleiro[hashMap.getValueOf(posicaoInicial[0])] - posTabuleiro[hashMap.getValueOf(posicaoFinal[0])], 0, 0);
        delay(50);
        sr.step(meiaCasa, 0, 1);
        delay(50);
       }
    }
    
    else{
      if((int) posicaoFinal[1] - 48 > (int) posicaoInicial[1] - 48){
        sr.step(meiaCasa, 1, 0);
        delay(50);
        sr.step(posTabuleiro[(int) posicaoFinal[1] - 48 - 1] - posTabuleiro[(int) posicaoInicial[1] - 48 - 1], 1, 1);
        delay(50);
        sr.step(meiaCasa, 0, 0);
        delay(50);
      }
      else if((int) posicaoFinal[1] - 48 < (int) posicaoInicial[1] - 48){
       sr.step(meiaCasa, 1, 0);
       delay(50);
       sr.step((posTabuleiro[(int) posicaoInicial[1] - 48 - 1] - posTabuleiro[(int) posicaoFinal[1] - 48 - 1]), 0, 1);
       delay(50);
       sr.step(meiaCasa, 0, 0);
       delay(50);
     }
   }
    Serial.println("Certo 3");
    servo.write(0);
    delay(1000);
    sr.step(posTabuleiro[hashMap.getValueOf(posicaoFinal[0])], 0, 0);
    delay(50);
    sr.step(posTabuleiro[(int) posicaoFinal[1] - 48 - 1], 0, 1);
    delay(50);
    numeroDeJogadas++;
}

void atacarPeca(String posicaoInicial, String posicaoFinal){
    sr.step(posTabuleiro[hashMap.getValueOf(posicaoFinal[0])], 1, 0);
    delay(50);
    sr.step(posTabuleiro[(int) posicaoFinal[1] - 48 - 1], 1, 1);
    delay(50);
    servo.write(73);
    delay(1000);
    if(numeroDeJogadas % 2 == 1)
    {
      if(posCemiterioY[cemiterioJ2] > posTabuleiro[(int) posicaoFinal[1] - 48 - 1])
      {
        sr.step(meiaCasa, 1, 1);
        delay(50);
        if(cemiterioJ2 > 8){
          sr.step(9 * umaCasa - posTabuleiro[hashMap.getValueOf(posicaoFinal[0])], 1, 0);
        } else {
          sr.step(8 * umaCasa - posTabuleiro[hashMap.getValueOf(posicaoFinal[0])] , 1, 0);
        }
        delay(50);
        sr.step(posCemiterioY[cemiterioJ2] - posTabuleiro[(int) posicaoFinal[1] - 48 - 1] - meiaCasa, 1, 1);
        delay(50);
        sr.step(meiaCasa, 1, 0);
        delay(50);
      }
      if (posCemiterioY[cemiterioJ2] < posTabuleiro[(int) posicaoFinal[1] - 48 - 1])
      {
        sr.step(meiaCasa, 0, 1);
        delay(50);
        if(cemiterioJ2 > 8){
          sr.step(posTabuleiro[hashMap.getValueOf(posicaoFinal[0])] + umaCasa, 1, 0);
        } else {
          sr.step(8* umaCasa - posTabuleiro[hashMap.getValueOf(posicaoFinal[0])], 1, 0);
        }    
        delay(50);
        sr.step(posTabuleiro[(int) posicaoFinal[1] - 48 - 1] - posCemiterioY[cemiterioJ2] - meiaCasa, 0, 1);
        delay(50);
        sr.step(meiaCasa, 1, 0);
        delay(50);
      }
       if (posCemiterioY[cemiterioJ2]  ==  posTabuleiro[(int) posicaoFinal[1] - 48 - 1])
      {
        sr.step(meiaCasa, 1, 1);
        delay(50);
        if(cemiterioJ2 > 8){
          sr.step(9 * umaCasa - posTabuleiro[hashMap.getValueOf(posicaoFinal[0])] + meiaCasa, 1, 0);
        } else {
          sr.step(8 * umaCasa - posTabuleiro[hashMap.getValueOf(posicaoFinal[0])] + meiaCasa, 1, 0);
        }
        delay(50);
        sr.step(meiaCasa, 0, 1);
        delay(50);
      }
     
      servo.write(0);    
      delay(1000);
      sr.step(posTabuleiro[cemiterioJ2], 0, 1);
      delay(50);
      if(cemiterioJ2 > 8){
        sr.step(posCemiterioX[3] + meiaCasa, 0, 0);
      } else {
        sr.step(posCemiterioX[1] + meiaCasa, 0, 0);
      }
      delay(50);
       cemiterioJ2++;
    }
    else if (numeroDeJogadas % 2 == 0)
    {
      if(posCemiterioY[cemiterioJ1] > posTabuleiro[(int) posicaoFinal[1] - 48 - 1])
      {
        sr.step(meiaCasa, 1, 1);
        delay(50);
        if(cemiterioJ1 > 8){
          sr.step(posTabuleiro[hashMap.getValueOf(posicaoFinal[0])] + umaCasa, 0, 0);
        } else {
          sr.step(posTabuleiro[hashMap.getValueOf(posicaoFinal[0])], 0, 0);
        }
        delay(50);
        sr.step(posCemiterioY[cemiterioJ1] - posTabuleiro[(int) posicaoFinal[1] - 48 - 1] - meiaCasa, 1, 1);
        delay(50);
        sr.step(meiaCasa, 0, 0);
        delay(50);
      }
      if (posCemiterioY[cemiterioJ1] < posTabuleiro[(int) posicaoFinal[1] - 48 - 1])
      {
        sr.step(meiaCasa, 0, 1);
        delay(50);
        if (cemiterioJ1 > 8){
           sr.step(posTabuleiro[hashMap.getValueOf(posicaoFinal[0])] + umaCasa, 0, 0);
        } else {
           sr.step(posTabuleiro[hashMap.getValueOf(posicaoFinal[0])], 0, 0);
        }
        delay(50);
        sr.step(posTabuleiro[(int) posicaoFinal[1] - 48 - 1] - posCemiterioY[cemiterioJ1] - meiaCasa, 0, 1);
        delay(50);
        sr.step(meiaCasa, 0, 0);
        delay(50);
      }
       if (posCemiterioY[cemiterioJ1]  ==  posTabuleiro[(int) posicaoFinal[1] - 48 - 1])
      {
        sr.step(meiaCasa, 1, 1);
        delay(50);
        if (cemiterioJ1 > 8){
           sr.step(posTabuleiro[hashMap.getValueOf(posicaoFinal[0])] + meiaCasa + umaCasa, 0, 0);
        } else {
           sr.step(posTabuleiro[hashMap.getValueOf(posicaoFinal[0])] + meiaCasa, 0, 0);
        }
        delay(50);
        sr.step(meiaCasa, 0, 1);
        delay(50);
      }
      
      servo.write(0);
      delay(1000);
      sr.step(posTabuleiro[cemiterioJ1], 0, 1);
      delay(50);
      if(cemiterioJ1 > 8){
        sr.step(posCemiterioX[2], 1, 0);
      } else {
        sr.step(posCemiterioX[0], 1, 0);
      }
      delay(50);
      cemiterioJ1++;
    }
    
    moverPeca(posicaoInicial, posicaoFinal);
}

void Reset()
{
  while(botaoX != HIGH)
    {
      sr.step(1,0,0);
    }
  while(botaoY != HIGH)
  {
    sr.step(1,0,1);
  }
}

