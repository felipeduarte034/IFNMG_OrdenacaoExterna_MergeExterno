#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <cmath>

//ALUNO: Luiz Felipe Duarte Fiuza Pereira

using namespace std;

/// HEAP SORT FOR STRING
void TrocaStr(vector<string> &v, int a, int b)
{
    string memAux;
    memAux = v[a];
    v[a] = v[b];
    v[b] = memAux;
}

void MaxHeapfyStr(vector<string> &v, int pos, int n)
{
    int maior=2*pos+1, right=maior+1;

    if(maior<n)
    {
        if(right<n && (strcmp(v[maior].c_str(), v[right].c_str()) < 0))
            maior = right;
        if(strcmp(v[maior].c_str(), v[pos].c_str()) > 0)
        {
            TrocaStr(v,maior,pos);
            MaxHeapfyStr(v,maior,n);
        }
    }
}

void BuildMaxHeapStr(vector<string> &v, int n)
{
    for(int i=n/2-1; i>=0; i--)
    {
        MaxHeapfyStr(v,i,n);
    }
}

void HeapSortStr(vector<string> &v, int n)
{
    BuildMaxHeapStr(v,n);
    for(int i=n-1; i>0; i--)
    {
        for(int i=n-1; i>0; i--)
        {
            TrocaStr(v,0,i);
            MaxHeapfyStr(v,0,--n);
        }
    }
}

void StringHeapSort(vector<string> &registers)
{
    HeapSortStr(registers, registers.size());
}

/// CREATE FILE TEST
string RandomText()
{
    string text;
    int numLetras = 3+(rand()%17);
    //return numLetras;
    for(int i=0; i<numLetras; i++)
    {
        text += 'a'+(char)(rand()%26);
    }
    int numSpaces = 19-numLetras;
    for(int i=0; i<numSpaces; i++)
    {
        text +=" ";
    }
    //cout << text << endl;
    return text;
}

string RandomNumber()
{
    string txt;
    for(int i=0; i<4; i++)
    {
        txt += 48+(char)(rand()%10);
    }
    return txt;
}

bool CreateTestFile(char* name_file, int quant_reg)
{
    string nome;
    string telefone;
    ofstream fileW;
    fileW.open(name_file, ios::out | ios::app | ios::binary);
    if(!fileW.is_open())
    {
        cout <<  "ALERT: Não foi possivel abrir/criar o arquivo! nome: " << name_file << endl;
        return false;
    }
    for(int i=0; i<quant_reg; i++)
    {
        string nome = RandomText();
        string telefone = RandomNumber() + "-" + RandomNumber();
        string item = nome + ":" + telefone;
        fileW << item << endl;
    }
    fileW.close();

    return true;
}

/// MERGE EXTERNO
bool PreencherVetorComPrimeirosPedacos(string* v, int tam, int* creg, char* name_file_temp, int rb, int rp)
{
    ifstream fileIn;
    string line;
    int counterReg=0;
    int block=0;
    int i=0;
    int position=0;
    int tamBlockOcuped = 30*rb;
    position = tamBlockOcuped;

    fileIn.open(name_file_temp);
    if(fileIn.is_open()) /// Verifica se o arquivo externo existe
    {
        while(getline(fileIn,line))
        {
            v[i] = line;
            i++;
            counterReg++;
            creg[block] += 1;
            if(counterReg==rp)
            {
                counterReg=0;
                block++;
                fileIn.seekg(position, ios::beg);
                position += tamBlockOcuped;
            }
        }
    }
    else
    {
        cout <<  "ALERT! nao foi possivel abrir o arquivo: " << name_file_temp << endl;
    }
    fileIn.close();
    return true;
}

bool LoadNextPiece(string* v, int* creg, int* ind, int block, int rb, int rp, char* name_file_temp)
{
    ifstream fileIn;
    string line;
    int posIni = (creg[block]*30)+(block*rb*30); ///posicao inicial do cursor no arquivo
    int counterLines=0;
    int posVetor=rp*block;

    fileIn.open(name_file_temp);
    if(fileIn.is_open()) /// Verifica se o arquivo externo existe
    {
        fileIn.seekg(posIni, ios::beg);
        while(getline(fileIn,line))
        {
            if(counterLines<rp && creg[block]<rb)
            {
                ///cout << "  line: " << line << "  posicao vetor: " << posVetor << "  block : " << block << endl;
                v[posVetor] = line;
                posVetor++;
                creg[block] += 1;
                counterLines++;
            }
            else
                break;
        }
    }

    fileIn.close();
    if(counterLines>0)
        return true;

    return false;
}

string GetMenor(string* v, int* ind, int* creg, int* creg2, int k, int rp, int rb, int rlp, int rlb, int* pCrlb, char* name_file_temp, int* pBlock)
{
    string menor="";
    /*
    cout<<endl;
    for(int i=0; i<k; i++)
    {
        int id = (i*rp)+ind[i];
        if(ind[i]!=-1)
            cout << "(" << i << ") " << v[id] << "  id: " << id << "  ind: " << ind[i] << endl;
    }
    */

    for(int i=0; i<k; i++) ///seta a variavel menor, com um valor aleatorio.
    {
        if(ind[i] >= 0)
        {
            int id = (i*rp)+ind[i];
            menor=v[id];
            *pBlock=i;
            break;
        }
    }
    ///cout << " menor ANTES: " << menor << endl;

    for(int i=0; i<k; i++) ///recupera qual o registro com menor valor.
    {
        if(ind[i] == -1) ///verificar se o bloco já chegou ao fim
            continue;

        int id = (i*rp)+ind[i];
        if(strcmp(v[id].c_str(), menor.c_str()) < 0)
        {
            menor = v[id];
            *pBlock=i;
        }
    }
    ///cout << " menor DEPOIS: " << menor << endl;

    if(*pBlock<k && menor!="")
    {
        ind[*pBlock] += 1;
        if(ind[*pBlock] == rp) ///verifica se chegou ao fim do pedaço
            ind[*pBlock] = -1;
        if(*pBlock == (k-1) && rlp < rp) ///verifica se é o ultimo bloco
        {
            *pCrlb += 1;
            if(*pCrlb==rlb)
                ind[*pBlock] = -1;
        }
        if(*pBlock != (k-1))
        {
            creg2[*pBlock] += 1;
            if(creg2[*pBlock] == rb)
                ind[*pBlock] = -1;
        }
    }

    return menor;
}

bool MergeExterno(char* name_file_temp, char* name_file_sort, int rlp, int rlb, int rf, int rb, int rp, int k)
{
    ifstream fileIn;
    ofstream fileOut;

    string* vetorMerge;
    string* bufferOut;
    int* ind; ///vetor auxiliar, para salvar a posiçao do registro no pedaço carregado na memoria.
    int* creg; ///vetor auxiliar, para contar quantos registros de cada bloco, já foram carregados.
    int counter=0;
    bool isSort=false;
    int h=0; ///indece buffer de saida
    int auxBlock=-1;
    int* pBlock=&auxBlock;
    int auxRlb=0;
    int* pCrlb=&auxRlb; ///contador para o ultimo bloco, para ultimo pedaço com menos registros que 'rp'
    int* creg2; ///vetor auxiliar, para contar quantos registros de cada bloco, já foram usados, salvos no arquivo de saida.

    vetorMerge=new string[k*rp]; ///quantidade de blocos vezes a quantidade de registros por pedaço.
    bufferOut=new string[rp];
    ind=new int[k];
    creg=new int[k];
    creg2=new int[k];

    for(int i=0; i<k; i++)
    {
        ind[i]=0;
        creg[i]=0;
        creg2[i]=0;
    }

    ///preencher vetorMerge
    PreencherVetorComPrimeirosPedacos(vetorMerge,k*rp,creg,name_file_temp,rb,rp);

    ///realiza o merge
    fileOut.open(name_file_sort, ios::out | ios::app | ios::binary);
    if(fileOut.is_open()) /// Verifica se o arquivo externo existe
    {
        while(!isSort)
        {
            string menor = GetMenor(vetorMerge,ind,creg,creg2,k,rp,rb,rlp,rlb,pCrlb,name_file_temp,pBlock);
            if(menor != "")
            {
                counter++;
                ///cout << " pBlock: " << *pBlock << " creg: " << creg[*pBlock] << "  ind: " << ind[*pBlock] << "  counter: " << counter << endl;
                bufferOut[h]=menor;
                h++;
                if(ind[*pBlock]==-1 && creg[*pBlock] < rb)/// && creg[*pBlock] < rb
                {
                    int resp = LoadNextPiece(vetorMerge,creg,ind,*pBlock,rb,rp,name_file_temp);
                    if(resp)
                    {
                        ind[*pBlock]=0;
                    }
                }
            }

            if(h==rp) ///descarrega os registros do buffer de saida no arquivo
            {
                for(int i=0; i<h; i++)
                {
                    fileOut << bufferOut[i] << "\n";
                }
                h=0;
            }
            /*
            cout << " ind: ";
            for(int i=0; i<k; i++)
            {
                cout << "(" << i << ")" << ind[i] << "  ";
            }
            cout << endl;
            */

            if(counter==rf)
            {
                for(int i=0; i<h; i++)
                {
                    fileOut << bufferOut[i] << "\n";
                }
                h=0;

                isSort=true;
                fileOut.close();
                remove(name_file_temp);
                return true;
            }
        }
    }
    else
    {
        cout <<  "ALERT! nao foi possivel abrir o arquivo: " << name_file_sort << endl;
    }
    fileOut.close();
    return false;
}

/// ORDENAÇÂO EXTERNA
bool SaveBlockInFile(vector<string> &v, char* name_fileout)
{
    ofstream fileTempW;

    fileTempW.open(name_fileout, ios::out | ios::app | ios::binary);
    if(fileTempW.is_open())
    {
        for(int i=0; i < (int)v.size(); i++)
            fileTempW << v[i];
    }
    fileTempW.close();
    return true;
}

bool OrdenaBlocos(char* name_file_ext, char* name_file_temp, int rf, int rb, int k)
{
    ifstream fileExt;
    ofstream fileTemp1W;
    int counter=0;
    char* line = new char [30];
    vector<string> registers;
    ///int aux=0;

    fileExt.open(name_file_ext);
    if(fileExt.is_open()) /// Verifica se o arquivo externo existe
    {
        for(int i=0; i<k; i++) ///quantidade de blocos
        {
            for(int j=0; j<rb; j++) ///quantidade de registros por bloco
            {
                fileExt.read(line,30);
                if(counter<rf)
                {
                    registers.push_back(line);
                    counter++;
                }
                else ///se atingir a quantidade de registros no arquivo, termina a funçao.
                {
                    ///for(int s=0; s<(int)registers.size(); s++)
                    ///    cout << ++aux << " : " << registers[s];
                    StringHeapSort(registers);
                    SaveBlockInFile(registers,name_file_temp);
                    registers.clear();
                    return true;
                }
            }
            ///for(int s=0; s<(int)registers.size(); s++)
            ///    cout << ++aux << " : " << registers[s];
            StringHeapSort(registers);
            SaveBlockInFile(registers,name_file_temp);
            registers.clear();
        }
    }
    else
    {
        cout <<  "ALERT! nao foi possivel abrir o arquivo: " << name_file_ext << endl;
    }
    fileExt.close();
    return false;
}

int getTamReg(char* name_file)
{
    ifstream fileExt;
    string line;

    fileExt.open(name_file);
    if(fileExt.is_open()) /// Verifica se o arquivo externo existe
    {
        while(getline(fileExt,line))
        {
            fileExt.close();
            return line.size();
        }
    }
    else
    {
        cout <<  "ALERT! nao foi possivel abrir o arquivo: " << name_file << endl;
    }
    fileExt.close();
    return -1;
}

bool RunExternalOrder(float arg_mem, char* arg_name_file)
{
    ifstream fileExt;
    ifstream fileTemp1;
    ifstream fileSort;
    ofstream fileTemp1W;
    ofstream fileSortW;

    char* nameFileTemp1 = "temp1.txt";
    char* nameFileSort = "sort.txt";
    char namefinal[100];
    strcat(namefinal,arg_name_file);
    strcat(namefinal,".ordenado");

    int tamReg=1;
    int tamFileExt=0;
    int tamBuffer=0;
    int k=0; ///quantidade de blocos
    int rf=0; ///quantidade de registros no arquivo
    int rb=0; ///quantidade de registros no bloco
    int rp=0; ///quantidade de registros por pedaço
    int rlb=0; ///quantidade de registros no ultimo bloco
    int rlp=0; ///quantidade de registro no ultimo pedaço, do ultimo bloco

    fileExt.open(arg_name_file);
    if(!fileExt.is_open()) /// Verifica se o arquivo externo existe
    {
        cout <<  "ALERTA: Nao foi possivel ler o arquivo externo! nome: " << arg_name_file << endl;
        return false;
    }

    ///se os arquivos já existirem, são limpos.
    fileSortW.open(nameFileSort, ios::trunc);
    fileSortW.close();
    fileTemp1W.open(nameFileTemp1, ios::trunc);
    fileTemp1W.close();

    ///calculos uteis
    tamReg += getTamReg(arg_name_file);
    if(arg_mem<tamReg)
    {
        cout <<  "ALERT! Tamanho do BLOCO menor que o tamanho do REGISTRO. Tente aumetar a memoria fornecida." << endl;
        return false;
    }
    fileExt.seekg(0,ios::end);
    tamFileExt = fileExt.tellg();
    fileExt.seekg(0,ios::beg);
    fileExt.close();
    rf=tamFileExt/tamReg;
    rb=arg_mem/tamReg;
    k=ceil((float)rf/(float)rb);
    tamBuffer=arg_mem/(k+1);
    if(tamBuffer<tamReg)
    {
        cout <<  "ALERT! Tamanho do PEDACO (buffer) menor que o tamanho do REGISTRO. Tente aumetar a memoria fornecida." << endl;
        return false;
    }
    if(arg_mem>tamFileExt)
    {
        cout <<  "ALERT! Memoria fornecida maior que o tamanho do arquivo externo. Use Ordenação Interna." << endl;
        return false;
    }
    rp=tamBuffer/tamReg;
    rlb = rf-((k-1)*rb);
    int pk = ceil((float)rlb/(float)rp); ///quantidade de pedaços no ultimo bloco
    rlp = rlb-((pk-1)*rp);
    ///*
    cout << " tamReg: " << tamReg << " bytes" << endl;
    cout << " rf: " << rf << endl;
    cout << " k: " << k << " blocos" << endl;
    cout << " rb: " << rb << endl;
    cout << " rlb: " << rlb << endl;
    cout << " tamBuffer: " << tamBuffer << " bytes" << endl;
    cout << " rp: " << rp << endl;
    cout << " pk: " << pk << endl;
    cout << " rlp: " << rlp << endl;
    ///*/

    bool isPart1 = OrdenaBlocos(arg_name_file,nameFileTemp1,rf,rb,k);
    if(!isPart1)
        return false;
    bool isPart2 = MergeExterno(nameFileTemp1,nameFileSort,rlp,rlb,rf,rb,rp,k);
    if(!isPart2)
        return false;

    ///realizar o rename no arquivo de saida ordenado
    rename(nameFileSort,namefinal);

    return true;
}


int main(int argc, char* argv[])
{
    char* name_file="";
    int tam_mem_max=0;
    if(argc<3)
    {
        cout << "Ops! parametros insuficientes para inicar o programa: \n--> primeiro: MEMORIA. \n--> segundo: NOME_ARQUIVO." << endl;
        return 0;
    }
    tam_mem_max = atoi(argv[1]);
    name_file = argv[2];
    ///cout << " tam_mem_max: " << tam_mem_max << endl;
    ///cout << " name_file: " << name_file << endl;
    cout << endl << "ORDENACAO INICIADA=============================================" << endl;
    bool resp = RunExternalOrder(tam_mem_max, name_file);

    /*
    char* name11 = "in-11-registros.txt";
    char* name100 = "in-100-registros.txt";
    char* name1M = "in-1M-registros.txt";
    int mem_1MB = 1*1000*1000;
    int mem_500B = 500;
    int mem_60B = 60;

    ///CreateTestFile(name1M, 1000000); ///Cria arquivo de teste no formato exigido.

    cout << endl << "ORDENACAO INICIADA=============================================" << endl;
    bool resp = RunExternalOrder(500, name100);
    */

    cout << endl << "RESULTADO======================================================" << endl;
    if(resp)
        cout <<  "--> Ordenacao externa concluida com SUCESSO!" << endl;
    else
        cout <<  "--> Ops! Ordenacao externa nao pode ser concluida..." << endl;

    return 0;
}
