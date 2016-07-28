//
// Created by Shao.Admin on 16/5/13.
//

#ifndef BOOST_TCPCLIENT_HUMAN_H
#define BOOST_TCPCLIENT_HUMAN_H

enum Color
{
    Red,
    Green,
    Black
};

class Human {

public:

    void Walk();


    void See(Color color);
};


#endif //BOOST_TCPCLIENT_HUMAN_H
