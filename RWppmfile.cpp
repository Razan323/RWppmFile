#include <iostream>
#include <fstream>
#include <string>
using namespace std;
// 1...
struct pixels {
    unsigned int red , green , blue;
};
// 2...
class Image{
    private: 
    char  mag_num1, mag_num2;
    unsigned int width, height , max_color;
    pixels **p;
    void alloc_raster(){
        if ((width > 0) && (height > 0) && (p == NULL)){
            p = new pixels*[height]; 
            for (unsigned int k = 0; k <height; k++) 
                p[k] = new pixels[width];
        }
    }
    public: 
    Image() : mag_num1('p'), mag_num2('3'), width(0), height(0), max_color(255), p(NULL)
    {   }
    void read_ppm(string file_name){
        ifstream inFile;
        inFile.open(file_name);
        if(!inFile.is_open()){
        cerr << "error opening file " << file_name << endl;
        }
        else {
            cout<<" "<<file_name<<" opened ..."<<endl;
            mag_num1 = inFile.get();
            inFile.get(mag_num2);
            inFile >> width;
            inFile >> height;
            inFile >> max_color;
            string h,e,f,d;
            inFile >> h>>e>>f>>d;
            cout<<"        read header of file ..."<<endl;
            alloc_raster();
            cout<<"        allocate "<<height<<"x"<<width<<" pixel array on heap ... done"<<endl;
            while(!inFile.eof()){
                for(int i=0; i<height; i++){
                for(int j=0; j<width; j++){
                    inFile >> p[i][j].red;
                    inFile >> p[i][j].green;
                    inFile >> p[i][j].blue;
                }
            }
            } 
        inFile.close();
        cout<<"        read RGB pixel data row by row ..."<<endl;
        cout<<"        done and "<<file_name<<" closed"<<endl;
    }
    }
    void write_ppm(string file_name){
        ofstream outFile;
        outFile.open(file_name);
        if(!outFile.is_open()){
        cerr << "error opening file " << file_name << endl;
        }
        else {
            cout<<" "<<file_name<<" opened ..."<<endl;
            cout<<"        write header of file ..."<<endl;
            outFile << mag_num1 << mag_num2 <<endl;
            outFile << width << " " << height <<endl;
            outFile << max_color <<endl;
            outFile << "# end of header" <<endl;
            for (int i=0; i<height; i++){
                for(int j=0; j<width; j++){
                outFile << p[i][j].red <<" ";
                outFile << p[i][j].green<<" ";
                outFile << p[i][j].blue;
                outFile.put('\n');
                }
            }
            outFile.close();
            cout<<"        write RGB pixel data row by row ..."<<endl;
            cout<<"        done and "<<file_name<<" closed"<<endl;
        }  
        outFile.flush();
    }
    void mirror_vertically(){
        cout<<"     start mirror image vertically ..."<<endl;
        for(int i=0; i < height; ++i){
            for(int j=0; j < width/2; ++j){
                 pixels temp;
                 temp = p[i][j];
                 p[i][j] = p[i][width -1 -j];
                 p[i][width -1 -j] = temp;
            }
        }
        cout<<"     done."<<endl;;
    }
    void grayscale(){
        cout<<"      start grayscale filter..."<<endl;
        for(int i=0; i<height; i++){
            for(int j=0; j<width; j++){
                p[i][j].red =  ( p[i][j].red + p[i][j].green + p[i][j].blue)/3;
                p[i][j].green =  ( p[i][j].red + p[i][j].green + p[i][j].blue)/3;
                p[i][j].blue =  ( p[i][j].red + p[i][j].green + p[i][j].blue)/3;
            if(p[i][j].red > 255)  p[i][j].red = 255; 
            if(p[i][j].green > 255) p[i][j].green = 255; 
            if(p[i][j].blue > 255) p[i][j].blue = 255;    
            }
        }
        cout<<"       done.";
    }
    void frame(const unsigned int w, unsigned int R,unsigned int G,unsigned int B){
    cout<<"         start generating frame with frame width "<<w<<"...."<<endl;
    for(int i=0; i<height; i++){
                for(int k=0; k<w; k++){
                    p[i][k].red=R;
                    p[i][k].green=G;
                    p[i][k].blue=B;
                }
        }
        for(int i=0; i<height; i++){
                for(int k=width-w; k<width; k++){
                    p[i][k].red=R;
                    p[i][k].green=G;
                   p[i][k].blue=B;
                }
        }
        for(int i=0; i<w; i++){
                for(int k=0; k<width; k++){
                    p[i][k].red=R;
                    p[i][k].green=G;
                    p[i][k].blue=B;
                }
        }
        for(int i=height-w; i<height; i++){
                for(int k=0; k<width; k++){
                    p[i][k].red=R;
                    p[i][k].green=G;
                    p[i][k].blue=B;
                }
        }

    cout<<"       done.";
    }
    ~Image() {
        for(int i=0; i<height; i++){
        delete [] p[i];}
        delete [] p;
    }
};
int main(void){
    Image i;
    int y;
    unsigned int widthOfFrame, R, G, B;
    string f_name, f_name2;
    do{
        cout<<"0 end / end"<<endl;
        cout<<"1 read .ppm file / read .ppm file"<<endl;
        cout<<"2 mirror vertically / mirror vertically"<<endl;
        cout<<"3 write .ppm file / write .ppm file"<<endl;
        cout<<"4 Graustufen / grayscale"<<endl;
        cout<<"5 Rahmen / frame"<<endl;
        cin>>y;
        switch(y)
        { 
            case 0:
                break;
            case 1: cout<<"please input file name: ";
            cin>>f_name;
            cout<<"read_ppm: ";
            i.read_ppm(f_name);
                break;
            case 2:
            i.mirror_vertically();
                break;
            case 3:
            cout<<"please input file name: ";
            cin>>f_name2;
            cout<<"write_ppm: ";
            i.write_ppm(f_name2);
                break;
            case 4:
            i.grayscale();
                break;
            case 5:
            cout<<"please input width of the frame: ";
            cin>>widthOfFrame;
            cout<<"please input R G B values: ";
            cin>>R>>G>>B;
            i.frame(widthOfFrame, R, G, B);
                break;
            default: 
            cerr << "Error! Please enter a valid choice" << endl;
        }
    }while(y);
    return 0;
}