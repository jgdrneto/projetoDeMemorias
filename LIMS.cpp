#ifndef _LIMS_
#define _LIMS_

#include <string>

#include "API_Processos.cpp"

using namespace std;

#define CACHE "Cached:"

class LIMS{

private:
  	int memoria;
  	int cache;
  	int swap;
  	int mjfaults;
  	int mifaults;
  	vector<Processo> processos;

  	void zerarValores(){
  		cache=0;
  		memoria=0;
  		swap=0;
  		mjfaults=0;
		mifaults=0;
  	}

  	void setValoresProcessos(){

  		for(Processo p : this->processos){
  			this->memoria+=p.getMemoria();
  			this->swap+=p.getSwap();
  			this->mjfaults+=p.getMjfaults();
			this->mifaults+=p.getMifaults();
  		}
  	}

  	/*  Descrição : Função que quebra string em varias string pelo caractere de " " 
   	 *
   	 *  @param const string& : String que se deseja quebrar
   	 *  
   	 *  @return vector<string> : Vetor de string resultantes da quebra
   	 */

  	vector<string> split(const string& str) {
    	stringstream ss(str);
    	return {istream_iterator<string>{ss}, istream_iterator<string>{}};
  	}

  	/*  Decrição : Obtem um vetor de string resultante de um comando
   	 *
   	 *  @param string : Nome do arquivo
   	 *
   	 *  @return vector<string> : Vetor com string de cada linha do arquivo
   	 */ 
  	vector<string> obterStringsDoComando(string comando){

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

  	/*  Descrição : Obteem os atributos relevantes das informações  
   	 *
   	 *  @param vector<string> : Vetor com as informações brutas
   	 *  
   	 *  @return vector<string> : Vetor com as informações interpretadas 
   	 */
  	map<string,string> obterAtributosChaveValor(vector<string> informacoes){
    
    	//Vetor com as inforamções relevantes
    	map<string,string> atributos;

    	//Interando entre todas as linhas do vetor das informações
    	for(string s : informacoes){

	      	//Quebrando as informações
	      	vector<string> lexemas = split(s);
	      
	      	if (lexemas[0] == "Cached:"){
	        	atributos[CACHE] = lexemas[1];  
	      	}
    	}

    	return atributos;
  	}

  	void setCache(){
  		map<string,string> atributosMemoria = obterAtributosChaveValor(obterStringsDoComando("cat /proc/meminfo"));
  		
  		this->cache = std::stoi(atributosMemoria[CACHE],nullptr,0);
  	}

public:
  	
  	LIMS(){

  		update();
  	}

  	void update(){

  		zerarValores();

  		processos = API_Processos::obterProcessos();

  		setValoresProcessos();

  		setCache();

  	}

  	string getInformacoesRelevantes(){
  		
    	stringstream informacoes;

    	informacoes << "";

  		if(this->processos.size()>1){

	  		Processo pMM = this->processos[1].getMemoria();
	  		Processo pMS = this->processos[1].getSwap();
	  		Processo pMMj = this->processos[1].getMjfaults();
	  		Processo pMMi = this->processos[1].getMifaults();

	  		for(Processo p : this->processos){
	  			if(p.getMemoria()>pMM.getMemoria()){
	  				pMM = p;
	  			}
	  			if(p.getSwap()>pMS.getSwap()){
	  				pMS = p;
	  			}
	  			if(p.getMjfaults()>pMMj.getMjfaults()){
	  				pMMj = p;
	  			}
	  			if(p.getMifaults()>pMMi.getMifaults()){
	  				pMMi = p;
	  			}
	  		}

	  		informacoes << "Processo de maior uso de memória: \t" 		<< pMM.getId() << "\t Valor: \t" <<  pMM.getMemoria() << "\n" <<
	  				   	   "Processo de maior uso de SWAP: \t \t"			<< pMS.getId() << "\t Valor: \t" <<  pMS.getSwap() << "\n" <<
	  				   	   "Processo que tem maior Major faults: \t"	<< pMMj.getId() << "\t Valor: \t" <<  pMMj.getMjfaults() << "\n" <<
	  				   	   "Processo que tem maior Minor faults: \t"	<< pMMi.getId() << "\t Valor: \t" <<  pMMi.getMifaults();

	  	}	

	  	return informacoes.str();
  	}

    /*  Descrição : Obter a mémoria usada por todos os processos
     *
     *  @return unisgned int : Valor em KB de memória usada por todos
     */
    int getMemoria(){
      return this->memoria;  
    }

    /*  Descrição : Obter o valor da memória de todos os processos em Swap
     *
     *  @return int : Valor da memória em swap
     */
    int getSwap(){
      return this->swap;
    }

    /*  Descrição : Obter o valor da da cache do sistema
     *
     *  @return int : Valor da cache
     */
    int getCache(){
      return this->cache;
    }

    /*  Descrição : Obter o valor de major faults de todos os processos
     *
     *  @return int : Valor de major faults de todos os processos
     */
    int getMjfaults() {
      return this->mjfaults;
    }

    /*  Descrição : Obter o valor de minor faults de todos os processos
     *
     *  @return int : Valor de minor faults de todos os processos
     */
    int getMifaults() {
      return this->mifaults;
    }

};

#endif  	