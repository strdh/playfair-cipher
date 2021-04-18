#include<iostream>
#include<string>

using namespace std;

void getKey(char box[5][5], string key){
    //proses untuk menghapus spasi dan huruf J, jika terdapat pada key
    for(int i = 0; i<key.length(); i++){
        if (toupper(key.at(i)) == 'J' || !isalpha(key.at(i))) {
            key.erase(i, 1);
            i--;
        }
    }
    //proses menghapus duplikasi huruf pada key
    string res = ""; 
    int key_len = key.length(), res_len = 0;
    res.push_back(toupper(key.at(0)));
    for(int i = 0; i<key_len; i++){
        res_len = res.length();
        for(int  j = 0; j<res_len; j++){
            if (toupper(key.at(i)) == res.at(j))
                break;
            if(j == res_len - 1)
                res.push_back(toupper(key.at(i)));
        }
    }
    //menambhakan huruf - huruf alfabet yang belum ada pada key (kecuali J) kedalam key
    for(int i = 65; i<=90; i++){
        for(int j = 0; j<res.length(); j++){
            if(res.at(j) == i || i == 74)
                break;
            if(j == res.length() - 1)
                res.push_back(i);
        }
    }
    //memasukan key kedalam box (atau bujursangkar)
    for(int i = 0, res_iter = 0; i<5; i++){
        for(int j = 0; j<5; j++){
            box[i][j] = res.at(res_iter);
            res_iter++;
        }
    }
}

string encrypt(string plainteks, char box[5][5]){
    string result = "";
    string fix_plain = "";
    int p_len = plainteks.length();
    //proses untuk mengganti huruf J dengan huruf I (jika ada huruf J pada plainteks)
    //dan menyisipkan huruf jika pada bigram terdapat huruf yang sama
    for(int i = 0; i<p_len; i++){
        char c = plainteks.at(i);
        if(!isalpha(c))
            continue;
        c = toupper(c);
        if(c == 'J'){
            fix_plain += 'I';
        }else{
            int n = fix_plain.length();
            if ((n & 1) and (c == fix_plain.at(n - 1))){
                fix_plain += 'X';
            }

            fix_plain += c;
        }
    }

    p_len = fix_plain.length();
    //jika panjang plainteks ganjil maka tambahkan huruf X di akhir
    if(p_len & 1){
        fix_plain += 'X';
        p_len++;
    }

    int x, y;
    //berikut adalah proses subtitusi
    for (int i = 0; i < p_len; ++i){
        char c = fix_plain.at(i);
        for (int k = 0; k < 5; ++k){
            for (int l = 0; l < 5; ++l){
                if (box[k][l] == c){
                    if (i & 1){
                        if (x == l){
                            result += box[(y + 1) % 5][x];
                            result += box[(k + 1) % 5][l];
                        }
                        else if (y == k){
                            result += box[y][(x + 1) % 5];
                            result += box[k][(l + 1) % 5];
                        }
                        else{
                            result += box[y][l];
                            result += box[k][x];
                        }
                    }
                    else{
                        y = k;
                        x = l;
                    }

                    k = 5;
                }
            }
        }
    }
    return result;
}

string decrypt(string cipher, char box[5][5]){
    string plainteks = "";
    int p_len = cipher.length();
    int x, y;
    //proses subtitusi
    for (int i = 0; i < p_len; ++i){
        char c = cipher.at(i);

        for (int k = 0; k < 5; ++k){
            for (int l = 0; l < 5; ++l){
                if (box[k][l] == c){
                    if (i & 1){
                        if (x == l){
                            y = y - 1;
                            k = k - 1;

                            y = (y < 0 ? y + 5 : y);
                            k = (k < 0 ? k + 5 : k);

                            plainteks += box[y][x];
                            plainteks += box[k][l];
                        }
                        else if (y == k){
                            x = x - 1;
                            l = l - 1;

                            x = (x < 0 ? x + 5 : x);
                            l = (l < 0 ? l + 5 : l);

                            plainteks += box[y][x];
                            plainteks += box[k][l];
                        }
                        else{
                            plainteks += box[y][l];
                            plainteks += box[k][x];
                        }
                    }
                    else{
                        y = k;
                        x = l;
                    }

                    k = 5;
                }
            }
        }
    }

    return plainteks;
}

int main(){
    string key;
    char box[5][5];
    int menu;
    char exit = 'y';
    do
    {
        cout<<"PILIH MENU\n1.Enkripsi\n2.Dekripsi\nPilih menu : ";
        cin>>menu;
        cin.ignore();

        cout<<"Input key       : ";
        getline(cin, key);
        getKey(box, key);

        if (menu == 1)
        {
            string plainteks;
            cout<<"Input plainteks : ";
            getline(cin, plainteks);
            string cipher = encrypt(plainteks, box);
            cout<<"Cipher          : "<<cipher<<"\n\n";
        }else if(menu == 2){
            string cipher;
            cout<<"Input cipher    : ";
            getline(cin, cipher);
            string plainteks = decrypt(cipher, box);
            cout<<"Plainteks       : "<<plainteks<<"\n\n";
        }

        cout<<"Apakah anda ingin lanjut ? (y/n) ";
        cin>>exit;
        
    } while (exit != 'n');
    
    return 0;
}