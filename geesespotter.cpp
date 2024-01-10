#include "geesespotter_lib.h"
#include <iostream>

char *create_board(std::size_t x_dim, std::size_t y_dim){
    char *board{new char[x_dim*y_dim]};
    for (unsigned int  i=0; i<x_dim*y_dim; i++){
        board[i]=(char)0;
    }

    return board;
}

void clean_board(char *board){
    delete[] board;
    board=nullptr;
}

void print_board(char *board, std::size_t x_dim, std::size_t y_dim){
     for (unsigned int  i=0; i<x_dim*y_dim; i++){
        if((board[i]&hidden_mask())==hidden_mask()){
            if((board[i]&marked_mask())==marked_mask()){
                std::cout<<"M";
            }
             else{
                std::cout<<"*"; 
                //std::cout<<(board[i]&value_mask());  
            }                     
        }
        else{       
            std::cout<<(board[i]&value_mask());   
        }
        
        if ((i+1)%x_dim==0){
            std::cout<<std::endl;
        }
    }
}

void hide_board(char *board, std::size_t x_dim, std::size_t y_dim){
    for (unsigned int  i=0; i<x_dim*y_dim; i++){
        board[i]|=hidden_mask();
    }
}

int mark(char *board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc, std::size_t y_loc){
    int x;
   for (unsigned int  i=0; i<x_dim*y_dim; i++){
        if(board[i]==board[x_loc+(x_dim*y_loc)]){
            if((board[x_loc+(x_dim*y_loc)]&hidden_mask())==hidden_mask()){
                x=0;
                if((board[x_loc+(x_dim*y_loc)]&marked_mask())==marked_mask()){
                    board[x_loc+(x_dim*y_loc)]^=marked_mask();
                    return 0;
                }
                else{
                    board[x_loc+(x_dim*y_loc)]|=marked_mask();
                    return 0;
                }
            }
            else{
                x=2;
            }
        }
    } 
    return x;
}

void compute_neighbours(char *board, std::size_t x_dim, std::size_t y_dim){
    for (unsigned int i=0; i<x_dim*y_dim; i++){
        int neighbournum{0};
        if ((board[i]&value_mask())!=9){
            int xcs;
            int xce;
            int ycs;
            int yce;
            //left
            if(((i%x_dim)==0)){
                xcs=0;
                xce=1;
            }
            //right
            else if(((i+1)%x_dim)==0){
                xcs=-1;
                xce=0;
            }
            //middle
            else{
                xcs=-1;
                xce=1;
            }
            //top
            if((i<x_dim)){
                ycs=0;
                yce=1;
            }
            //bottom
            else if(i+x_dim>(x_dim*y_dim-1)){
                ycs=-1;
                yce=0;
            }
            //middle
            else{
                ycs=-1;
                yce=1;
            }
            //compute
            for (int j=xcs; j<=xce; j++){
                for(int n=ycs; n<=yce; n++){
                    if((board[i+n*x_dim+j]&value_mask())==9){
                        neighbournum+=1;
                    }
                }
            }

        //change to number of neighbours
        board[i]^=neighbournum;  
        }
    }       
}  

bool is_game_won(char *board, std::size_t x_dim, std::size_t y_dim){
    int geesenum{0};
    int revealed{0};
    int markedgeese{0};


    for(unsigned int  i=0; i<x_dim*y_dim; i++){
        //num geese
        if((board[i]&value_mask())==9){
            geesenum+=1;
            //num geese marked
            if  ((board[i]&marked_mask())==marked_mask()){
                markedgeese+=1;
            }
        }
        else{
            //num marked
            if((board[i]&hidden_mask())!=hidden_mask()){
                revealed+=1;
            }
        }
    }

    //if all none geese fields revealed
    if((revealed+geesenum)==x_dim*y_dim){
        return true;
    }

    //if all geese marked
    if(geesenum==markedgeese){
        return true;
    }

    return false;
}

    
int reveal(char *board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc,std::size_t y_loc){   
    //marked
    if ((board[x_loc+(x_dim*y_loc)]&marked_mask())==marked_mask()){
        return 1;
    }
    //goose
    if((board[x_loc+(x_dim*y_loc)]&value_mask())==9){
        board[x_loc+(x_dim*y_loc)]^=hidden_mask(); 
        return 9;
    }
    
    //revealed
    if((board[x_loc+(x_dim*y_loc)]&hidden_mask())!=hidden_mask()){
        return 2;
    }

    //reveal
    board[x_loc+(x_dim*y_loc)]^=hidden_mask();

    //zero
    if((board[x_loc+(x_dim*y_loc)]&value_mask())==0){
        int b=x_loc+(x_dim*y_loc);
        int xcs;
        int xce;
        int ycs;
        int yce;
        //left
        if(((b%x_dim)==0)){
            xcs=0;
            xce=1;
        }
        //right
        else if(((b+1)%x_dim)==0){
            xcs=-1;
            xce=0;
        }
        //middle
        else{
            xcs=-1;
            xce=1;
        }
        //top
        if(b<x_dim){
            ycs=0;
            yce=1;
        }
        //bottom
        else if((b+x_dim)>(x_dim*y_dim-1)){
            ycs=-1;
            yce=0;
        }
        //middle
        else{
            ycs=-1;
            yce=1;
        }
        //compute
        for (int j=xcs; j<=xce; j++){
            for(int n=ycs; n<=yce; n++){
                if(((board[b+n*x_dim+j]&marked_mask())!=marked_mask())&&((board[b+n*x_dim+j]&hidden_mask())==hidden_mask())){
                    board[b+n*x_dim+j]^=hidden_mask(); 
                }       
            }
        }    
    } 
    return 0;       
}

//g++ -std=c++11 geesespotter.cpp geesespotter_lib.cpp -o main
