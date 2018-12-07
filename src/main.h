//
// Created by pekne on 6.12.18.
//

#ifndef SRC_HEADERS_H
#define SRC_HEADERS_H

#endif //SRC_HEADERS_H

//#define DEBUG
#ifdef DEBUG
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x) do {} while (0)
#endif

//#define STATS
#ifdef STATS
# define STATS_PRINT(x) printf x
#else
# define STATS_PRINT(x) do {} while (0)
#endif


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
