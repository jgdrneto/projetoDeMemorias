#ifndef _API_PROCESSOS_
#define _API_PROCESSOS_

//Bibliotecas
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <iterator>
#include <unistd.h>
#include <map> 

//Classe com o nome do processo
#include "Processo.cpp"

#include "json.hpp"

#define ARQUIVO_TEMP "temp.txt"       //Nome do arquivo temporário

#define NAME "Name:"
#define PAI "PPid:"
#define USUARIO "Uid:"
#define MEMORIA "VmData:"
#define MEMSWAP "VmSwap:"
#define MJFAULTS "MjFaults:"
#define MIFAULTS "MiFaults:"
#define MJFAULTSNUMBER 11
#define MIFAULTSNUMBER 9

class API_Processos{
private:
  /*  Decrição : Obtem um vetor de string resultante de um comando
   *
   *  @param string : Nome do arquivo
   *
   *  @return vector<string> : Vetor com string de cada linha do arquivo
   */ 
  static vector<string> obterStringsDoComando(string comando){

    stringstream ss;

    ss << comando << " > " << ARQUIVO_TEMP;
      
    //Executando comando
    system(ss.str().c_str());  

    //Vetor que terá as linhas do comando
    vector<string> linhas;
    string linha; 
    ifstream file;
    //Abrindo arquivo com as informações do comando executado pelo system()
    file.open(ARQUIVO_TEMP);

    if (file.is_open()){
          
      while ( getline (file,linha) ) {
        //Adicionando linha no vetor de string
        linhas.push_back(linha);
      }
     file.close();
    } 
    
    //Retornando um vetor de string onde cada string é uma linha do resultado(no terminal) do comando system();
    return linhas;

  }

  /*  Descrição: Obter o comando para ter as informações de um determinado processo
   *  
   *  @param string : Id do processo que se deseja ter as informações  
   *
   *  return string : Comando que retorna as informações de um processo
   */
  static string obterComInformacaoProcesso(string processo){

    //String no qual vai está o comando
    string comando;
    //String capaz de se fazer concatenação
    stringstream ss;
    //Concatenando string para gerar o comando
    ss << "cat /proc/" << processo << "/status";

    //Atribuindo o valor da concatenação em uma string
    return ss.str();
  }

  /*  Descrição : Construir a árvore
   *  
   *  @param vector<Processo>& : Endereço de vetor de processos de todos os processos
   */
  static void construirArvore(vector<Processo>& processos){
    
    for(unsigned int pai=0;pai<processos.size();pai++){
      for(unsigned int filho=0;filho<processos.size();filho++){
        if(processos[pai].getId() == processos[filho].getPai()){

          processos[pai].getFilhos()->push_back(&(processos[filho]));

        }
      }
    }

  }

   /*  Descrição : Função que quebra string em varias string pelo caractere de " " 
   *
   *  @param const string& : String que se deseja quebrar
   *  
   *  @return vector<string> : Vetor de string resultantes da quebra
   */

  static vector<string> split(const string& str) {
      stringstream ss(str);
      return {istream_iterator<string>{ss}, istream_iterator<string>{}};
  }

  /*  Descrição : Obteem os atributos relevantes das informações  
   *
   *  @param vector<string> : Vetor com as informações brutas
   *  
   *  @return vector<string> : Vetor com as informações interpretadas 
   */
  static map<string,string> obterAtributosChaveValor(vector<string> informacoes){
    
    //Vetor com as inforamções relevantes
    map<string,string> atributos;

    //Interando entre todas as linhas do vetor das informações
    for(string s : informacoes){

      //Quebrando as informações
      vector<string> lexemas = split(s);
      
      if (lexemas[0] == NAME    ||
          lexemas[0] == PAI     ||
          lexemas[0] == USUARIO ||
          lexemas[0] == MEMORIA ||
          lexemas[0] == MEMSWAP 
          ){
            atributos[lexemas[0]] = lexemas[1];  
      }

    }

    return atributos;
  }

  /*  Descrição : Obteem os atributos relevantes das informações  
   *
   *  @param vector<string> : Vetor com as informações brutas
   *  
   *  @return vector<string> : Vetor com as informações interpretadas 
   */
  static map<string,string> obterAtributosValor(vector<string> informacoes){
    
    //Vetor com as inforamções relevantes
    map<string,string> atributos;
    vector<string> lexemas;
    vector<string> temp;


    //Interando entre todas as linhas do vetor das informações
    for(string s : informacoes){

      temp = split(s);

      //Quebrando as informações
      lexemas.insert(lexemas.end(),temp.begin(),temp.end());

    }
    
    /* 
    for(string s : lexemas){
      cout << s << endl;
    }

    cout << "====================================" << endl;
    */
      
    atributos[MIFAULTS] = lexemas[9];
    atributos[MJFAULTS] = lexemas[11];  

    return atributos;
  }

  /*  Descrição : Verificar integridade das informações
   *
   *  @param vector<string> : String com todos os processos
   *
   *  @return vector<string> : String que passaram no controle de integridade 
   *
   */
  static vector<string> verificarIntegridade(vector<string> processos){
    
    //Vetor resultante 
    vector<string> result;
    
    //Iterando entre todos os processos  
    for(string p : processos){

      ifstream obj;
      obj.open("/proc/"+p+"/status");

      //Aceitando apenas as strings que tem o arquivo status  
      if (obj.is_open()) {
        result.push_back(p);  
      }
      obj.close();
    }

    return result;
  }

  /*  Descrição : Obter informações do processo
   *
   *  @param string : String com id do processo
   *
   *  @return vector<string> : Obter infomrações do processo
   */
  static vector<string> obterinformacoesProcesso(string processo,string nomeArquivo){
    vector<string> linhas;
    
    string linha; 
    ifstream file;
    
    file.open("/proc/"+processo+"/"+nomeArquivo);
    
    if (file.is_open()){
      
      while ( getline (file,linha) ) {

        linhas.push_back(linha);

      }
     file.close();
    }else{

      vector<string> s;
      return s;
    
    } 
    
    //Resultado do comando
    return linhas;
  }

  /*  Descrição : Construir os processos
   *
   *  @return vector<Processo> : Vetor com os processo presentes no sistema operacional
   */
  static vector<Processo> construirProcessos(){
    
    vector<Processo> resultado;

    //Criando processo Odin(Pai de todos)
    resultado.push_back(*(new Processo(0,"OS",-1,-1, (unsigned int)0)));

    //Obtendo os id de todos os processos  
    vector<string> processosString = verificarIntegridade(obterStringsDoComando("ls /proc | grep \"^[0-9]\""));  

    //Iterando entre todos os ids encontrados
    for(string p : processosString){
      
      //Obter inforamções do processo  
      vector<string> informacoesStatus = obterinformacoesProcesso(p,"status");
      vector<string> informacoesStat = obterinformacoesProcesso(p,"stat");

      //Se existir informações sobre ele adiciona-o para o vetor resultante
      if(!informacoesStatus.empty() && !informacoesStatus.empty()){

        //Obter os atributos relevantes
        map<string,string> atributosStatus = obterAtributosChaveValor(informacoesStatus);
        
        map<string,string> atributosStat = obterAtributosValor(informacoesStat);
        
        Processo* proc = new Processo(std::stoi(p,nullptr,0));
        
        /*
        cout << "ID: " << p << endl;
        cout << "Nome: " << atributos[NAME] << endl;
        cout << "Pai: " << atributos[PAI] << endl;
        cout << "Usuário: " << atributos[USUARIO] << endl;
        cout << "Memória: " << atributos[MEMORIA] << endl;
        cout << "SWAP: " << atributos[MEMSWAP] << endl;
        */

        if(!atributosStatus[NAME].empty()){
          proc->setNome(atributosStatus[NAME]);
        }
        if(!atributosStatus[PAI].empty()){
          proc->setPai(std::stoi(atributosStatus[PAI],nullptr,0));
        }
        if(!atributosStatus[USUARIO].empty()){
          proc->setUsuario(std::stoi(atributosStatus[USUARIO],nullptr,0));
        }
        if(!atributosStatus[MEMORIA].empty()){
          proc->setMemoria((unsigned int)std::stoi(atributosStatus[MEMORIA],nullptr,0));
        }
        if(!atributosStatus[MEMSWAP].empty()){
          proc->setSwap(std::stoi(atributosStatus[MEMSWAP],nullptr,0));
        }
        
        //Informação de falta de páginas
        proc->setMifaults(std::stoi(atributosStat[MIFAULTS],nullptr,0));  
        proc->setMjfaults(std::stoi(atributosStat[MJFAULTS],nullptr,0));
        
        resultado.push_back(*proc);
      }
    }   
    return resultado;
  }

public:
  /*  Descrição : Imprimir a árvore em pré-ordem 
   *
   *  @param Processo& : Endereço do processo raiz
   */
  static void imprimirArvore(Processo& raiz){

    //Imprimindo processo atual
    cout << "Id: " << raiz.getId() << " " << "Nome: " << raiz.getNome() << " Pai: " << raiz.getPai() << " Usuário: " << raiz.getUsuario() << endl; 

    //Iterando em todos os processos filhos
    for(Processo* p : *(raiz.getFilhos())){

      //Recursão que imprime os filhos do processo
      imprimirArvore(*(p));

    }

  }

  /*  Descrição : Buscar o processo pelo id
   *
   *  @param string : Id do processo que se deseja obter 
   *  @param vector<Processo>& : Endereço de vetor de processos de todos os processos
   *
   *  @return Processo& : Processo que se deseja buscar
   */
  static Processo& buscaPorId(int id,vector<Processo>& processos){

    for(unsigned int i=0;i<processos.size();i++){
        if(id==processos[i].getId()){
          return processos[i];
        } 
    }

    return *(new Processo(-1,"erro", -1, -1, (unsigned int)0));

  }

  /*  Descrição : Verifica se existe um usuario em uma lista
   *
   *  @param vector<int> : Vetor com os usuários
   *  @param string : Usuário que se quer saber se já está no vetor
   *
   *  @return bool : Verdadeiro se id usuário está no vetor, e falso caso contrário
   */
  static bool contemUsuario(vector<int> usuarios,int s ){

    for(int u : usuarios){
      if(u==s){
        return true;
      }
    }

    return false;
  }

  /*  Descrição : Obter todos os usuários distintos dos processos
   *  
   *  @param vector<Processo> : Vetor com todos os processos
   *
   *  @return vector<string> : Vetor com todos os usuários distintos do processo
   */
  static vector<int> obterUsuariosDistintos(vector<Processo> processos){
    
    vector<int> resultado;

    //Iterando entre os processos
    for(Processo p : processos){
      //Excluindo o processo Odin(Pai de todos)
      if(p.getUsuario()!=-1){
        //Adicionando apenas usuários que ainda não existe no vetor
        if(!contemUsuario(resultado,p.getUsuario())){
          resultado.push_back(p.getUsuario());
        }
      }
    }

    return resultado;
  }

  /*  Descrição : Contar todos os processo de um determindado usuário
   *
   *  @param int : Id do usuário que se deseja ver a quantidade de procesos
   *  @param vector<Processo> : Todos os processos que serão analisados
   *
   *  @return int : Quantidade de processos de um usuário 
   */

  static int contarProcessosDoUsuario(int usuario, vector<Processo> processos){

    int cont=0;
    //Iterando entre todos os usuários
    for(Processo p :processos){
      //Contando apenas os que tem usuário igual
      if(p.getUsuario()==usuario){
        cont++;
      }
    }

    return cont;
  }

  /*  Descrição : Salvar os processos em um arquivo JSON
   *
   *  @param vector<Processos> : Processos que serão salvos
   */
  static void salvarArvore(vector<Processo> processos){
    
    nlohmann::json j;

    //Iterando entre todos os processos
    for(Processo p : processos){

      //Ecluindo o processo Odin(pai de todos)
      if(p.getId()!=0){    
        //Salvar as informações no fim do arquivo json 
        j.push_back(nlohmann::json{{"ID", p.getId()}, {"Nome", p.getNome()}, {"Pai", p.getPai()},{"Usuário", p.getUsuario()},{"Memória Utilizada", p.getMemoria()},{"Memória em SWAP", p.getSwap()}});
      }
    }
    
    //Escrita do objeto representante no JSON no arquivo JSON
    std::ofstream o("arvore.json");
    o << std::setw(4) << j << std::endl;
  }

  /*  Descrição : Imprimir a árvore de processos na tela
   */
  static void verArvore(){

    //Construindo todos os processos
    vector<Processo> processos = construirProcessos();

    //Construindo a árvore 
    construirArvore(processos);

    //Buscando processo pelo seu id
    Processo p = buscaPorId(0, processos);

    //Verificando se processo existe
    if(p.getId()!=-1){
      imprimirArvore(p);
    }else{
      cout << "Processo não existe" << endl;
    }

  }

  static vector<Processo> obterProcessos(){
     
     vector<Processo> processos = construirProcessos();
     construirArvore(processos);

     return processos;
  };

};

#endif