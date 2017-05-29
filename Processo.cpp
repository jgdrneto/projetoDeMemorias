#ifndef _PROCESSO_
#define _PROCESSO_

#include <string>
#include <vector>

using namespace std;

//Classe representativa de processos
class Processo{
  private:
  	//Atributos
  	int id;
  	string nome;
  	vector<Processo*> filhos;
  	int usuario;
  	int pai;
    unsigned int memoria;
    int swap;
    int mjfaults;
    int mifaults;

  public:

    Processo(){/*NULO*/}

  	Processo(int nId){
  		this->id = nId;
      this->memoria=0;
      this->swap = 0;
      this->mjfaults=0;
      this->mifaults=0;
  	}

    Processo(int nId,string nNome, int nUsuario, int nPai, unsigned int nMemoria){
      this->id = nId;
      this->nome = nNome;
      this->usuario = nUsuario;
      this->pai = nPai;
      this->memoria = nMemoria;
      this->swap = 0;
      this->mjfaults=0;
      this->mifaults=0;
    }

    /*  Descrição : Obter o id do processo
     *
     *  @return int : Id do processo
     */
  	int getId(){
  		return this->id;	
  	}

    /*  Descrição : Obter a mémoria usada pelo processo
     *
     *  @return unisgned int : Valor em KB de memória usada no processo
     */
    unsigned int getMemoria(){
      return this->memoria;  
    }

    /*  Descrição : Obter o id do pai do processo
     *
     *  @return int : Id do pai do processo
     */
  	int getPai(){
  		return this->pai;
  	}

    /*  Descrição : Obter o valor da memória do processo em Swap
     *
     *  @return int : Valor da memória em swap
     */
    int getSwap(){
      return this->swap;
    }

    /*  Descrição : Obter o nome do processo
     *
     *  @return string : Nome do processo
     */
  	string getNome(){
  		return this->nome;
  	}

    /*  Descrição : Obter os filhos do processo
     *
     *  @return vector<Processo*>* : Vetor com os endereços dos filhos do processo
     */
  	vector<Processo*>* getFilhos(){
  		return &(this->filhos);
  	}

    /*  Descrição : Obter o id do usuário do processo
     *
     *  @return int : Id do usuário do processo
     */
    int getUsuario() {
      return this->usuario;
    }

    /*  Descrição : Obter o valor de major faults do processo
     *
     *  @return int : Valor de major faults do processo
     */
    int getMjfaults() {
      return this->mjfaults;
    }

    /*  Descrição : Obter o valor de minor faults do processo
     *
     *  @return int : Valor de minor faults do processo
     */
    int getMifaults() {
      return this->mifaults;
    }

    /*  Descrição : Adicionar novo pai para o processo
     *
     *  @param int : Novo id do pai do processo
     */  
    void setPai(int nPai){
      this->pai = nPai;
    }

/*  Descrição : Adicionar novo valor da memória de swap no processo
     *
     *  @param int : Novo valor para a memória de swap
     */  
    void setSwap(int nSwap){
      this->swap = nSwap;
    }

    /*  Descrição : Adicionar novo valor de memória para o processo
     *
     *  @param unsigned int : Nova memória do processo
     */  
    void setMemoria(unsigned int nMemoria){
      this->memoria = nMemoria;
    }

     /*  Descrição : Adicionar novo id para o processo
     *
     *  @param int : Novo id do processo
     */  
  	void setId(int nId){
  		this->id = nId;
  	}

    /*  Descrição : Adicionar novo nome para o processo
     *
     *  @param string : Novo nome do processo
     */ 
  	void setNome(string nNome){
  		this->nome = nNome;
  	}

    /*  Descrição : Adicionar novo usuário para o processo
     *
     *  @param int : Novo usuário do processo
     */ 
  	void setUsuario(int nUsuario){
  		this->usuario = nUsuario;
  	}

    /*  Descrição : Adicionar novo valor de minor faults para o processo
     *
     *  @param int : Novo valor de minor faults do processo
     */ 
    void setMifaults(int nMifaults){
      this->mifaults = nMifaults;
    }

    /*  Descrição : Adicionar novo valor de major faults para o processo
     *
     *  @param int : Novo valor de major faults do processo
     */ 
    void setMjfaults(int nMjfaults){
      this->mjfaults = nMjfaults;
    }

    string toString(){

      stringstream ss;
      //Concatenando string para gerar o comando
      ss << "Id: " << this->id << "\n" << 
            "Nome: " << this->nome << "\n" << 
            "Usuario: " << this->usuario << "\n" << 
            "Pai: " << this->pai << "\n" <<
            "Memória: " << this->memoria << "\n" << 
            "Swap: " << this->swap << "\n" <<
            "Quantidade de filhos: " << this->filhos.size() << "\n" <<
            "Minor Faults: " << this->mifaults << "\n"  <<
            "Major Faults: " << this->mjfaults << "\n"; 
            
      return ss.str();        
    }

};	

#endif