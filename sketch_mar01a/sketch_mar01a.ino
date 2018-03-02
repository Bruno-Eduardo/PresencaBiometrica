/*
 *
 * Desenvolvido por Bruno Oliveira - bruno97br@gmail.com
 *  Ultimo update - 01/03/2017
 */

#define PROFESSOR 1

/*  TODO:
 *  Importar bibliotecas (RTC/Biometrico/Cartao) 
 *  Implementar digitalLida()/iniciarDia()/encerrarDia()/gravarPresenca()/horario()
 *  Implementar entrada e comparação de datas para quando o professor inserir a digital duas vezes no mesmo dia
 *  digitalLida()
 *      pegar no pc da FEM
 *  iniciarDia()
 *      
*/


//Global Var
String dataHj = "inicio";

//int digitalLida(){return 1;}
//void gravarPresenca(int leitura, String data){}
//void iniciarDia(){}
//void encerrarDia(){}

void setup() {
  int leitura;
  
  do{
    leitura = digitalLida();
  }while(leitura != PROFESSOR);
  iniciarDia();
}

void loop() {
  int leitura = digitalLida();
  if(leitura == PROFESSOR){
    encerrarDia();
  }
  else{
   gravarPresenca(leitura, dataHj);
  }

}

int digitalLida(){
  return 1;
}
void gravarPresenca(int leitura, String data){
  ;
}
void iniciarDia(){
  ;
}
void encerrarDia(){
  asm volatile ("  jmp 0");
}
