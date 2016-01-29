//
//  eraseLine.cpp
//  EraseLine
//
//  Created by George Godas on 1/27/16.
//  Copyright © 2016 George Godas. All rights reserved.
//

#include "eraseLine.hpp"



Mat erase(Mat src){
    Mat horizontal = prepare(src.clone(), 15, -3);
    
    // Mescla linhas apagadas com imagem com angulo corrigido
    addWeighted(src, 1.0, horizontal, 1.0, 0.0, horizontal);
    
    return horizontal;
}