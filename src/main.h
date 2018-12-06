//
// Created by pekne on 6.12.18.
//

#ifndef SRC_HEADERS_H
#define SRC_HEADERS_H

#endif //SRC_HEADERS_H


class Larva: public Process
{
public:
    void Behavior();
};

class Rodic: public Process
{
public:
    void Behavior();
};
class Krabice: public Process
{
public:
    void Behavior();
};
class RozdelAPanuj: public Process
{
public:
    void Behavior();
};


class Vejca: public Process
{
public:
    void Behavior();
};

class VejcaGenerator: public Event
{
public:
    void Behavior();
};
