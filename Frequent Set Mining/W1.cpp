#include<iostream>
#include<fstream>
#include<bits/stdc++.h>
#include<chrono>

using namespace std;
using namespace std::chrono;

vector<int> helper;

struct freq{
    vector<int> iset;
    double freq;
};


class Node{
    public:
    int item_no;
    int freq;
    Node* parent;
    vector<Node*> childs;

    Node(int id, int fr){
        item_no = id;
        freq = fr;
        parent = NULL;
    } 
};

bool comparater(struct freq q1, struct freq q2){ return (q1.freq > q2.freq);}

vector<freq> candidate_gen(vector<freq> fset){
    vector<freq> resul;
    for(int i= 0; i< fset.size()-1; i++){
        for(int j = i+1; j< fset.size(); j++){
            vector<int> t1 = fset[i].iset;
            vector<int> t2 = fset[j].iset;
            bool isokay = true;
            if(t1.size() > 1){
                for(int k= 0; k< t1.size()-1; k++){
                    if(t1[k] != t2[k]){
                        isokay = false;
                        break;
                    }
                }
            }

            if(isokay){
                vector<int> entry;
                int temp1 = t1[t1.size()-1];
                int temp2 = t2[t2.size()-1];
                if(temp1 > temp2){
                    entry = t2;
                    entry.push_back(temp1);
                }
                else{
                    entry = t1;
                    entry.push_back(temp2);
                }
                if(t1.size() > 1){
                    bool cankeep = true;
                    for(int k= 0; k< t1.size()-1; k++){
                        vector<int> temp = entry;
                        temp.erase(temp.begin()+k);
                        bool ispos = false;
                        for(int l= 0; l< fset.size(); l++){
                            if(fset[l].iset == temp){
                                ispos = true;
                                break;
                            }
                        }
                        if(ispos == false){
                            cankeep = false;
                            break;
                        }
                    }

                    if(cankeep == true){
                        struct freq entr;
                        entr.freq = 0;
                        entr.iset = entry;
                        resul.push_back(entr);
                    }
                }
                else if(t1.size() == 1){
                    struct freq entr;
                    entr.iset = entry;
                    entr.freq = 0;
                    resul.push_back(entr);
                }
            }
        }
    }
    return resul;
}

Node* search(Node* root, int id){
    for(int i= 0; i< root->childs.size(); i++){
        if(root->childs[i]->item_no == id){
            return root->childs[i];
        }
    }
    return NULL;
}



void apriori(vector<freq> temp, double sup, string file, vector<vector<vector<string>>> &outp){
    while(temp.size() > 0){
        vector<freq> Ck = candidate_gen(temp);
        ifstream basket(file);
        string line;                
        while(getline(basket, line)){
            istringstream iss(line);
            int number;
            set<int> items;
            while(iss >> number){items.insert(number);}

            for(int i= 0; i< Ck.size(); i++){
                vector<int> present = Ck[i].iset;
                bool IsOk = true;
                for(int j = 0; j< present.size(); j++){
                    if(items.find(present[j]) == items.end()){
                        IsOk = false;
                        break;
                    }

                }
                if(IsOk){Ck[i].freq++;}
            }
        }
        basket.close();
        for(int i= Ck.size()-1; i >= 0 ;i--){
            if(Ck[i].freq < sup){
                Ck.erase(Ck.begin() + i);
            }
        }

        for(int i = 0; i< Ck.size(); i++){
            vector<int> temp = Ck[i].iset;
            vector<string> temp1;
            for(int j = 0; j< temp.size(); j++){
                string req = to_string(temp[j]);
                temp1.push_back(req);
            }

            sort(temp1.begin(), temp1.end());
            int index = temp1[0][0] - '0';
            outp[index].push_back(temp1);
        }
        temp = Ck;
    }
}



// void ent_search(Node* root, int id, vector<Node*> &transac){
//     if(root->item_no == id){transac.push_back(root);}
//     else{
//         for(int i= 0; i< root->childs.size(); i++){ent_search(root->childs[i], id, transac);}
//     }
    
// }

void ftree(Node* root, vector<freq> ff, double sup, vector<vector<vector<string>>> &outp, int thd, map<int, vector<Node*>> &header){

    if(ff.size() == 0 || (root->childs.size() == 0 && root->item_no == 0)){return;}
    vector<Node*> transac = header[ff[thd].iset[0]]; // = new vector<Node*>();
    //ent_search(root, ff[thd].iset[0], transac);         
    
    //Gets the Transactions required.
    vector<freq> tran;
    int req_size = 0;
    for(int i= 0; i< transac.size(); i++){
        Node* curr = transac[i];
        int repeat = curr->freq;
        curr = curr->parent;
        vector<int> temp;
        while(curr->parent != NULL){
            // struct freq use;
            // use.freq = 0;
            // use.iset.push_back(curr->item_no);              
            req_size = max(req_size, curr->item_no);
            temp.push_back(curr->item_no);
            curr = curr->parent;
        }
        if(temp.size() > 0){
            reverse(temp.begin(), temp.end());
            // for(int j= 0; j< repeat; j++){
            //     tran.push_back(temp);
            // }
            struct freq use;
            use.iset = temp;
            use.freq = repeat;
            tran.push_back(use);
        }
    }

    vector<freq> freqset(req_size+1);
    for(int i= 0; i<= req_size; i++){
        struct freq use;
        use.freq =0;
        use.iset.push_back(i);
        freqset[i] = use;
    }

    for(int i= 0; i< tran.size(); i++){
        int t_size = tran[i].iset.size();
        for(int j= 0; j< t_size; j++){
            freqset[tran[i].iset[j]].freq += tran[i].freq;
        }
    }

    vector<vector<freq>> r_tran(tran.size());
    for(int i= 0; i< tran.size(); i++){
        vector<freq> temp;
        for(int j= 0; j< tran[i].iset.size(); j++){
            struct freq use;
            use.iset.push_back(tran[i].iset[j]);
            use.freq = freqset[tran[i].iset[j]].freq;
            temp.push_back(use);
        }
        r_tran.push_back(temp);
    }
    // for(int i= 0; i< tran.size(); i++){
    //     int t_size = tran[i].iset.size();
    //     for(int j = 0; j< t_size; j++){
    //         tran[i][j].freq = freqset[tran[i][j].iset[0]].freq;
    //     }
    // }
    for(int j= req_size ; j>= 0; j--){
        if(freqset[j].freq < sup){
            freqset.erase(freqset.begin() + j);
        }
    }

    sort(freqset.begin(), freqset.end(), comparater);
    Node* n_root = new Node(0,0);
    Node* nt_root = n_root;

    map<int, vector<Node*>> n_header;
    for(int i= 0; i< r_tran.size(); i++){
        vector<freq> use = r_tran[i];
        sort(use.begin(), use.end(), comparater);
        //cout << use.size() << "\n";
        for(int j= use.size()-1; j>=0; j--){
            if(use[j].freq < sup){use.erase(use.begin()+j);}
            else{break;}
        }

        if(use.size() > 0){
            int j= 0; 
            while(j < use.size() ){
                Node* sear = search(nt_root, use[j].iset[0]);
                if(sear != NULL){
                    //cout << sear->item_no << " " << use[j].iset[0] << "\n";
                    sear->freq += use[j].freq;
                    nt_root = sear;
                    j++;
                }
                else{
                    for(int k = j; k< use.size(); k++){
                        //cout << k << " " << i << " " << use[k].iset[0] <<  "\n";
                        Node* temp0 =  new Node(use[k].iset[0], use[k].freq);
                        n_header[use[k].iset[0]].push_back(temp0);
                        nt_root->childs.push_back(temp0);
                        temp0->parent = nt_root;
                        nt_root = temp0;
                    }
                    j = use.size();
                }
            }
            nt_root = n_root;
        }
    }

    for(int i = freqset.size()-1 ; i >=0; i--){
        int item = freqset[i].iset[0];
        
        helper.push_back(item);
        vector<string> tmpt;
        
        for(int j= 0; j< helper.size(); j++){
            string req = to_string(helper[j]);
            tmpt.push_back(req);
        }
        sort(tmpt.begin(), tmpt.end());
        int ind = tmpt[0][0] - '0';
        (outp)[ind].push_back(tmpt);
        ftree(n_root, freqset, sup, outp, i, n_header);
        //delete(n_root);
        helper.pop_back();
    }
    delete(n_root);
    return;

}

int main(int argc, char *argv[]){
    
    string file = argv[2];
    //string file = "help.dat";

    double sup = stod(argv[3]);
    //double sup= 20;

    
    ifstream basket(file);
    ifstream temp(file);

    if(!basket.is_open()){
        cout << "Unable to read the file" << "\n";
        return 0;
    }

    else{
        int size = 0;
        string line;
        double buckets= 0;
        while(getline(basket, line)){
            buckets++;
            istringstream is(line);
            int number;
            while(is >> number){
                if(size < number){size = number;}
                //size = max(size, number);
            }
        }

        sup = sup*buckets;          //Chnaging the given %sup to freq.
        sup = sup/100;

        vector<double> itemset(size+1, 0);      //frequenices of every item in the itemset.

        basket.close();        
        line = "";
        while(getline(temp, line)){
            istringstream hl(line);
            int numb;
            while(hl >> numb){
                //cout << numb << " ";
                itemset[numb]++;
            }
            
        }
        temp.close();

        vector<vector<vector<string>>> outp(10) ; //= new vector<vector<vector<string>>>(10);

        vector<freq> fset;
        for(int i= 1; i< itemset.size(); i++){
            if(itemset[i] >= sup){
                struct freq temp;
                vector<int> it(1, i);
                temp.iset = it;
                temp.freq = itemset[i];
                fset.push_back(temp);

                vector<string> it1(1);
                it1[0] = to_string(i);
                int index = it1[0][0] - '0';
                (outp)[index].push_back(it1);
            }
        }
        string algo = argv[1];
        
        if(algo == "-apriori"){
            
            auto start = high_resolution_clock::now();
            apriori(fset, sup , file ,  outp);
            auto end = high_resolution_clock::now();
            cout << (duration_cast<microseconds>(end-start).count()*0.001) << "\n";
        }
        else{
            
            Node* root = new Node(0,0);
            Node* t_root = root;
            //cout << sup << "\n";
            ifstream basket(file);
            string line;
            map<int, vector<Node*>> header;
            // for(int i=0; i< fset.size(); i++){
            //     header[i];
            // }
            while(getline(basket, line)){
                istringstream iss(line);
                vector<freq> items;
                int number;
                while(iss >> number){
                    struct freq help;
                    //cout << number << " ";
                    if(itemset[number] >= sup){
                        help.freq = itemset[number];
                        help.iset.push_back(number);
                        items.push_back(help);
                    }
                }
                sort(items.begin(), items.end(), comparater);
                int i= 0;
                while(i < items.size()){
                    //struct freq head = items[i];
                    Node* sear = search(t_root, items[i].iset[0]);

                    if(sear != NULL){
                        //cout << sear->item_no << "\n";
                        sear->freq += 1;
                        t_root = sear;
                        i++;
                    } 
                    else{
                        for(int j = i; j< items.size(); j++){
                            Node* nn = new Node(items[j].iset[0],1);
                            header[items[j].iset[0]].push_back(nn);
                            t_root->childs.push_back(nn);
                            nn->parent = t_root;
                            t_root = nn;
                        }
                        i = items.size();
                    }
                }
                t_root = root;
            }
            basket.close();
            sort(fset.begin(), fset.end(), comparater);


            auto start = high_resolution_clock::now();
            int counter = fset.size()-1;
            for(int j= counter; j>= 0; j--){
                helper.push_back(fset[j].iset[0]);
                ftree(root,fset, sup, outp, j, header);
                helper.pop_back();
            }
            delete(root);

            auto end = high_resolution_clock::now();
            cout << (duration_cast<microseconds>(end-start).count()*0.001 ) << "\n";
            
        }

        string outfile = argv[4];
        ofstream wf;
        wf.open(outfile);
        if(!wf){
            cout << "Cannot open file" << "\n";
            return 0;
        }
        else{
            for(int i= 0; i< outp.size(); i++){
                sort((outp)[i].begin(), (outp)[i].end());
                for(int j = 0; j< (outp)[i].size() ; j++){
                    int s1 = (outp)[i][j].size();
                    for(int k = 0; k< s1; k++){
                        wf << (outp)[i][j][k] << " ";
                    }
                    wf << endl;
                }
                
            }
            wf.close();
            // delete(outp);
        }
    }
    
}
