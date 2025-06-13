// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "CoreMinimal.h"
#include <cmath>
#include <stdio.h>
#include <numbers>
#include <vector>
#include "Math/AsianMath.h"

enum EIcosID : int
{
    ID_NORTH_POLE = 0,

    ID_FIRST_FACE = 1,
    ID_SECOND_FACE = 2,
    ID_THIRD_FACE = 3,
    ID_FOURTH_FACE = 4,
    ID_FIFTH_FACE = 5,
    ID_SIXTH_FACE = 6,
    ID_SEVENTH_FACE = 7,
    ID_EIGHTH_FACE = 8,
    ID_NINTH_FACE = 9,
    ID_TENTH_FACE = 10,

    ID_SOUTH_POLE = 11,

    TOTAL_ICOSAHEDRON_NUMBER = 12,

    ID_EMPTY = -1   // For pentagons missing 6th!
};

enum EIcosLevel : int
{
    L_NORTH_POLE = 0,
    L_FIRST_RING = 1,
    L_SECOND_RING = 2,
    L_SOUTH_POLE = 3,

    TOTAL_LEVEL_NUMBER = 4
};

inline EIcosID iid(int int_id) {
    if ((int_id >= 0) && (int_id <= 3)) {
        return static_cast<EIcosID>(int_id);
    }
    return EIcosID::ID_EMPTY;
}

enum EIcosFaceDirection : int
{
    DIR_TOP_RIGHT = 0,
    DIR_RIGHT = 1,
    DIR_DOWN_RIGHT_VDIR_DOWN = 2,
    DIR_DOWN_LEFT = 3,
    DIR_LEFT = 4,
    DIR_TOP_LEFT_VDIR_TOP = 5,

    TOTAL_DIRECTION_NUMBER = 6
};


static const int icoPentagonNeighbors[EIcosID::TOTAL_ICOSAHEDRON_NUMBER][EIcosFaceDirection::TOTAL_DIRECTION_NUMBER] = {


           /* T_R   R    D_R   D_L  L  T_L*/
    /* 0 */ { 2,    1,    5,   -1,  4,   3},
    /* 1 */ { 0,    2,    6,   10,  5,   0},
    /* 2 */ { 0,    3,    7,    6,  1,   0},
    /* 3 */ { 0,    4,    8,    7,  2,   0},
    /* 4 */ { 0,    5,    9,    8,  3,   0},
    /* 5 */ { 0,    1,   10,    9,  4,   0},
    /* 6 */ { 2,    7,   11,   11, 10,   1},
    /* 7 */ { 3,    8,   11,   11,  6,   1},
    /* 8 */ { 4,    9,   11,   11,  7,   1},
    /* 9 */ { 5,   10,   11,   11,  8,   1},
    /*10 */ { 1,    6,   11,   11,  9,   1},
    /*11 */ { 6,    7,    8,   -1,  9,  10}
};



enum EIcosVertexDirection : int
{
    VERT_DIR_X_MINUS = 0,
    VERT_DIR_Y_PLUS_X0 = 1,
    VERT_DIR_Y_PLUS_NONZERO = 2,
    VERT_DIR_DIAG_PLUS = 3,
    VERT_DIR_X_PLUS_NONZERO = 4,
    VERT_DIR_X_PLUS_Y0 = 5,
    VERT_DIR_Y_MINUS = 6,
    VERT_DIR_DIAG_MINUS = 7,

    TOTAL_VERT_DIRECTION_NUMBER = 8
};


static const int icoVertexNeighbors[EIcosID::TOTAL_ICOSAHEDRON_NUMBER][EIcosVertexDirection::TOTAL_VERT_DIRECTION_NUMBER] = {


           /* X-   Y+x0   Y+  DIA+    X+   X+y0     Y-   DIA-*/
    /* 0 */ { 2,    1,    1,   5,     4,    4,      3,   -1},


    /* 1 */ { 5,    0,    2,    2,    6,     6,    10,   -1},
    /* 2 */ { 1,    0,    3,    3,    7,     7,     6,   -1},
    /* 3 */ { 2,    0,    4,    4,    8,     8,     7,   -1},
    /* 4 */ { 3,    0,    5,    5,    9,     9,     8,   -1},
    /* 5 */ { 4,    0,    1,    1,   10,    10,     9,   -1},

    /* 6 */ { 1,    2,    2,    7,    7,    11,    10,   -1},
    /* 7 */ { 2,    3,    3,    8,    8,    11,     6,   -1},
    /* 8 */ { 3,    4,    4,    9,    9,    11,     7,   -1},
    /* 9 */ { 4,    5,    5,   10,   10,    11,     8,   -1},
    /*10 */ { 5,    6,    6,    6,    6,    11,     9,   -1},

    /*11 */ { 6,    7,    7,    8,    9,     9,    10,   -1}
};






inline static const FVector ALL_COORDINATES[12] = {
        {0, 0, 1}, // North pole 0

        {A_ONE_OVER_SQRT5 * 2.0 * std::cos(2.0 * PI * 0.0 / 5.0),
        A_ONE_OVER_SQRT5 * 2.0 * std::sin(2.0 * PI * 0.0 / 5.0),
        1 * A_ONE_OVER_SQRT5}, // First face 1 

        {A_ONE_OVER_SQRT5 * 2.0 * std::cos(2.0 * PI * 4.0 / 5.0),
        A_ONE_OVER_SQRT5 * 2.0 * std::sin(2.0 * PI * 4.0 / 5.0),
        1 * A_ONE_OVER_SQRT5}, // First face 2

        {A_ONE_OVER_SQRT5 * 2.0 * std::cos(2.0 * PI * 3.0 / 5.0),
        A_ONE_OVER_SQRT5 * 2.0 * std::sin(2.0 * PI * 3.0 / 5.0),
        1 * A_ONE_OVER_SQRT5}, // First face 3 

        {A_ONE_OVER_SQRT5 * 2.0 * std::cos(2.0 * PI * 2.0 / 5.0),
        A_ONE_OVER_SQRT5 * 2.0 * std::sin(2.0 * PI * 2.0 / 5.0),
        1 * A_ONE_OVER_SQRT5}, // First face 4 

        {A_ONE_OVER_SQRT5 * 2.0 * std::cos(2.0 * PI * 1.0 / 5.0),
        A_ONE_OVER_SQRT5 * 2.0 * std::sin(2.0 * PI * 1.0 / 5.0),
        1 * A_ONE_OVER_SQRT5}, // First face 5 


        {A_ONE_OVER_SQRT5 * 2.0 * std::cos(2.0 * PI * 4.0 / 5.0 + PI / 5.0),
        A_ONE_OVER_SQRT5 * 2.0 * std::sin(2.0 * PI * 4.0 / 5.0 + PI / 5.0),
        -1 * A_ONE_OVER_SQRT5}, // First face 6

        {A_ONE_OVER_SQRT5 * 2.0 * std::cos(2.0 * PI * 3.0 / 5.0 + PI / 5.0),
        A_ONE_OVER_SQRT5 * 2.0 * std::sin(2.0 * PI * 3.0 / 5.0 + PI / 5.0),
        -1 * A_ONE_OVER_SQRT5}, // First face 7

        {A_ONE_OVER_SQRT5 * 2.0 * std::cos(2.0 * PI * 2.0 / 5.0 + PI / 5.0),
        A_ONE_OVER_SQRT5 * 2.0 * std::sin(2.0 * PI * 2.0 / 5.0 + PI / 5.0),
        -1 * A_ONE_OVER_SQRT5}, // First face 8

        {A_ONE_OVER_SQRT5 * 2.0 * std::cos(2.0 * PI * 1.0 / 5.0 + PI / 5.0),
        A_ONE_OVER_SQRT5 * 2.0 * std::sin(2.0 * PI * 1.0 / 5.0 + PI / 5.0),
        -1 * A_ONE_OVER_SQRT5}, // First face 9

        {A_ONE_OVER_SQRT5 * 2.0 * std::cos(2.0 * PI * 0.0 / 5.0 + PI / 5.0),
        A_ONE_OVER_SQRT5 * 2.0 * std::sin(2.0 * PI * 0.0 / 5.0 + PI / 5.0),
        -1 * A_ONE_OVER_SQRT5}, // First face 10

        {0, 0, -1}, // South pole 11
};

struct FIcosaPentCoord
{
    EIcosID id;
    EIcosLevel id_level;

    FIcosaPentCoord(EIcosID id) {
        this->id = id;
        this->id_level = this->level();
    }

    bool isPole()
    {
        if (this->id == EIcosID::ID_NORTH_POLE) return true;
        if (this->id == EIcosID::ID_SOUTH_POLE) return true;
        return false;
    }

    EIcosLevel level() {

        int ret = static_cast<int>(id);

        ret = (ret + 4) / 5; // 0 -> 0     1-5 -> 1      6-10 -> 2      11 -> 3

        return static_cast<EIcosLevel>(ret);
    }

    static EIcosLevel idToLevel(EIcosID idIn) {

        int ret = static_cast<int>(idIn);

        ret = (ret + 4) / 5; // 0 -> 0     1-5 -> 1      6-10 -> 2      11 -> 3

        return static_cast<EIcosLevel>(ret);
    }

    // TODO remove
    static int correctID(int idIn, int level) //, int from_level
    {

        if ((level == EIcosLevel::L_NORTH_POLE) || (level == EIcosLevel::L_SOUTH_POLE))
        {
            return (5 * level + 3 - idIn);
        }
        //if (level == EIcosLevel::L_NORTH_POLE) return static_cast<int>(EIcosID::ID_NORTH_POLE);  // no pentagons at poles
        //if (level == EIcosLevel::L_SOUTH_POLE) return static_cast<int>(EIcosID::ID_SOUTH_POLE);  // no pentagons at poles


        // 1 -> (1+4) % 5 + 1 + 0 = 1
        // 0 -> (0+4) % 5 + 1 + 0 = 
        // 2 -> (2+4) % 5 + 1 + 0 = 2
        // .....
        // 6 -> 1 + 5
        // 5 -> 5 + 5 = 10
        return (idIn + 4) % 5 + 1 + 5 * (level - 1);
    }

    EIcosID upLeft() const
    {
        int ret_id = static_cast<int>(this->id);
        return static_cast<EIcosID>(icoPentagonNeighbors[ret_id][DIR_TOP_LEFT_VDIR_TOP]);
        
        /*int level = idToLevel(this->id);

        ret_id = ret_id - 5;
        level = level - 1;

        ret_id = correctID(ret_id, level);

        return iid(ret_id);*/
    }

    EIcosID upRight() const
    {
        int ret_id = static_cast<int>(this->id);
        return static_cast<EIcosID>(icoPentagonNeighbors[ret_id][DIR_TOP_RIGHT]);

        /*int level = idToLevel(this->id);
        UE_LOG(LogTemp, Warning, TEXT("ret_id: %d | level: %d (input to upRight() )\n"),
            static_cast<int>(ret_id),
            static_cast<int>(level)
        );

        ret_id = ret_id - 4;
        level = level - 1;

        UE_LOG(LogTemp, Warning, TEXT("ret_id: %d | level: %d (after move in upRight() )\n"),
            static_cast<int>(ret_id),
            static_cast<int>(level)
        );

        ret_id = correctID(ret_id, level);

        UE_LOG(LogTemp, Warning, TEXT("ret_id: %d | level: %d (after correction in upRight() )\n"),
            static_cast<int>(ret_id),
            static_cast<int>(level)
        );

        return iid(ret_id);*/
    }

    EIcosID downLeft() const
    {
        int ret_id = static_cast<int>(this->id);
        return static_cast<EIcosID>(icoPentagonNeighbors[ret_id][DIR_DOWN_LEFT]);

        /*int level = idToLevel(this->id);

        ret_id = ret_id + 4;
        level = level + 1;

        ret_id = correctID(ret_id, level);

        return iid(ret_id);*/
    }

    EIcosID downRight() const
    {
        int ret_id = static_cast<int>(this->id);
        return static_cast<EIcosID>(icoPentagonNeighbors[ret_id][DIR_DOWN_RIGHT_VDIR_DOWN]);

        /*int level = idToLevel(this->id);

        ret_id = ret_id + 5;
        level = level + 1;

        ret_id = correctID(ret_id, level);

        return iid(ret_id);*/
    }

    EIcosID right() const
    {
        int ret_id = static_cast<int>(this->id);
        return static_cast<EIcosID>(icoPentagonNeighbors[ret_id][DIR_RIGHT]);

        /*int level = idToLevel(this->id);

        UE_LOG(LogTemp, Warning, TEXT("ret_id: %d | level: %d (input to right() )\n"),
            static_cast<int>(ret_id),
            static_cast<int>(level)
        );

        ret_id = ret_id + 1;
        UE_LOG(LogTemp, Warning, TEXT("ret_id: %d | level: %d (after move )\n"),
            static_cast<int>(ret_id),
            static_cast<int>(level)
        );
        ret_id = correctID(ret_id, level);
        UE_LOG(LogTemp, Warning, TEXT("ret_id: %d | level: %d (after correct )\n"),
            static_cast<int>(ret_id),
            static_cast<int>(level)
            );
        return iid(ret_id);*/
    }

    EIcosID left() const
    {
        int ret_id = static_cast<int>(this->id);
        return static_cast<EIcosID>(icoPentagonNeighbors[ret_id][DIR_LEFT]);

        /*int level = idToLevel(this->id);

        ret_id = ret_id - 1;

        ret_id = correctID(ret_id, level);
        return iid(ret_id);*/
    }
};




class FIcosaVertexNeighbourEntry
{
public:
    struct FIcoseNeighCoordTransform
    {
        // For some reason Unreal does not have any adequate linear algebra libraries, BRUH
        // Use this for now
        FVector xMatRow;
        FVector yMatRow;

        FVector2D transformXY(FVector2D xy) const
        {
            return this->transformXY(xy.X, xy.Y);
        }
        FVector2D transformXY(double x, double y) const
        {
            FVector xy_one = FVector(x, y, 1.0);

            double x_new = this->xMatRow.Dot(xy_one);
            double y_new = this->yMatRow.Dot(xy_one);

            return FVector2D(x_new, y_new);
        }
    };
    
    inline static const FIcoseNeighCoordTransform ALL_VERTEX_COORD_TRANSFORM[EIcosID::TOTAL_ICOSAHEDRON_NUMBER] = {
        {{0, 0, 0}, {0, 0, 0}},    // Pole
        {{0, 1, -4}, {-1, 0, 4}},  // Up
        {{1, 0, -4}, {0, 1, 0}},   // Right
        {{1, 0, 0}, {0, 1, 4}},    // Down
        {{0, 0, 0}, {0, 0, 0}},    // DownLeft
        {{0, -1, 3}, {1, 0, 4}},   // Left
    };

    
    inline static void correctCoordinate(const FIcosaPentCoord& objIn, EIcosID& neighID, FVector2D& xy, const int N_division) 
    {
        EIcosLevel level = FIcosaPentCoord::idToLevel(neighID);

        UE_LOG(LogTemp, Warning, TEXT("correctCoordinate: X: %d, Y: %d, face: %d!\n"),
            (int)(xy.X), (int)(xy.Y),
            static_cast<int>(neighID)
        );

        // TODO convert to lookup
        EIcosID pole;
        if (level == EIcosLevel::L_FIRST_RING)
            pole = objIn.upRight();
        else 
            pole = objIn.downRight();

        EIcosID up = objIn.right();
        
        EIcosID left = objIn.left();

        EIcosID right;
        if (level == EIcosLevel::L_FIRST_RING)
            right = objIn.downRight();
        else
            right = objIn.downRight();

        EIcosID down;
        if (level == EIcosLevel::L_FIRST_RING)
            down = objIn.downLeft();
        else
            down = objIn.upLeft();


        // TODO make more beautiful!
        int X = static_cast<int>(xy.X);
        int Y = static_cast<int>(xy.Y);

        bool x0 = (X == 0);
        bool x_minusOne = (X == -1);
        bool xN = (xy.X == N_division);
        bool xOther = (!xN) & (!x_minusOne);

        bool y0 = (Y == 0);
        bool y_minusOne = (Y == -1);
        bool yN = (Y == N_division);
        bool yOther = (!y_minusOne) & (!yN);

        if (xOther & yOther) { // Inside, no change
            UE_LOG(LogTemp, Warning, TEXT("No change in correctCoordinate!\n")
                );
            return;
        } 
        
        if (x_minusOne & y_minusOne) { // Pentagon at bottom Left does not have a neighbour
            UE_LOG(LogTemp, Warning, TEXT("[-1,-1] empty detected in correctCoordinate!\n")
            );
            neighID = EIcosID::ID_EMPTY;
        }
        else if (x0 & yN) { // Goes to pole
            neighID = pole;
            xy = ALL_VERTEX_COORD_TRANSFORM[0].transformXY(xy);
            UE_LOG(LogTemp, Warning, TEXT("x == 0 and y == N, pole detected!\n")
            );
        }
        else if (yN) {
            UE_LOG(LogTemp, Warning, TEXT("y == N, up detected! [%d]->[%d]\n"),
                static_cast<int>(neighID), static_cast<int>(up)
            );
            neighID = up;
            xy = ALL_VERTEX_COORD_TRANSFORM[1].transformXY(xy);
            
        }
        else if (xN) {
            UE_LOG(LogTemp, Warning, TEXT("x == N, righ detected! [%d]->[%d]\n"),
                static_cast<int>(neighID), static_cast<int>(right)
            );
            neighID = right;
            xy = ALL_VERTEX_COORD_TRANSFORM[2].transformXY(xy);
        }
        else if (y_minusOne) {
            UE_LOG(LogTemp, Warning, TEXT("y == -1, down detected! [%d]->[%d]\n"),
                static_cast<int>(neighID), static_cast<int>(down)
            );
            neighID = down;
            xy = ALL_VERTEX_COORD_TRANSFORM[3].transformXY(xy);
        }
        else if (x_minusOne) {
            UE_LOG(LogTemp, Warning, TEXT("x == -1, left detected! [%d]->[%d]\n"),
                static_cast<int>(neighID), static_cast<int>(left)
            );
            neighID = left;
            xy = ALL_VERTEX_COORD_TRANSFORM[5].transformXY(xy);
        }
    }


    
};


struct FIcosaPointCoord
{
    FIcosaPentCoord coord = FIcosaPentCoord(EIcosID::ID_EMPTY);
    int X = 0;
    int Y = 0;
    int N_division = 1;
    FVector w_position = FVector::Zero();

    EIcosID upRight = EIcosID::ID_EMPTY, downRight = EIcosID::ID_EMPTY, right = EIcosID::ID_EMPTY;

    FIcosaPointCoord(EIcosID id, FVector2D xy, int N_division) :
        coord(FIcosaPentCoord(id)), X(xy.X), Y(xy.Y), N_division(N_division),
        upRight(this->coord.upRight()),
        downRight(this->coord.downRight()),
        right(this->coord.right())
    {

        this->N_division = this->N_division * !this->coord.isPole() + this->coord.isPole();
        if(static_cast<int>(this->coord.id) != -1)
            this->w_position = (computeCoordinate());
        /*
        UE_LOG(LogTemp, Warning, TEXT("[constructor FIcosaPointCoord] face: %d | Position: %f %f %f | N_division: %d\n"),
            static_cast<int>(id),
            this->w_position.X, this->w_position.Y, this->w_position.Z, N_division);

        UE_LOG(LogTemp, Warning, TEXT("upRight: %d | downRight: %d | right: %d\n"),
            static_cast<int>(upRight),
            static_cast<int>(downRight),
            static_cast<int>(right));
        */
    }

    FIcosaPointCoord(EIcosID idIn, int x, int y, int N_division) :
        FIcosaPointCoord(idIn, FVector2D(x, y), N_division)
    {
    }

    FVector computeCoordinate() {

        EIcosID otherDir;
        
        FVector X_dir;
        FVector Y_dir;
        
        if (X <= Y) {
            otherDir = upRight;

            Y_dir = (ALL_COORDINATES[static_cast<int>(otherDir)] - ALL_COORDINATES[static_cast<int>(coord.id)]);
            X_dir = (ALL_COORDINATES[static_cast<int>(right)] - ALL_COORDINATES[static_cast<int>(otherDir)]);
        }
        else {
            otherDir = downRight;

            X_dir = (ALL_COORDINATES[static_cast<int>(otherDir)] - ALL_COORDINATES[static_cast<int>(coord.id)]);
            Y_dir = (ALL_COORDINATES[static_cast<int>(right)] - ALL_COORDINATES[static_cast<int>(otherDir)]);
        }
        

        /*
        UE_LOG(LogTemp, Warning, TEXT("face: %d | X: %f | Y: %f | N_division: %f | otherDir: %d | right: %d | coord: %d\n"),
            static_cast<int>(coord.id),
            static_cast<double>(X), 
            static_cast<double>(Y),
            static_cast<double>(N_division), static_cast<int>(otherDir), static_cast<int>(right),
            static_cast<int>(coord.id)
        );

        UE_LOG(LogTemp, Warning, TEXT("otherID: [%f, %f, %f] - coord.idL [%f, %f, %f]\n"),
            ALL_COORDINATES[static_cast<int>(otherDir)].X,
            ALL_COORDINATES[static_cast<int>(otherDir)].Y,
            ALL_COORDINATES[static_cast<int>(otherDir)].Z,
            ALL_COORDINATES[static_cast<int>(coord.id)].X,
            ALL_COORDINATES[static_cast<int>(coord.id)].Y,
            ALL_COORDINATES[static_cast<int>(coord.id)].Z
        );

        UE_LOG(LogTemp, Warning, TEXT("static_cast<double>(%f) / static_cast<double>(%f) = %f\n"),
            static_cast<double>(X),
            static_cast<double>(N_division),
            static_cast<double>(X) / static_cast<double>(N_division)
        );
       
        FVector ret = ALL_COORDINATES[coord.id] +
            (static_cast<double>(X) / static_cast<double>(N_division)) * (ALL_COORDINATES[static_cast<int>(otherDir)] - ALL_COORDINATES[static_cast<int>(coord.id)]);

        UE_LOG(LogTemp, Warning, TEXT("[%f %f %f] + %f * [%f %f %f] = [%f %f %f]\n"),
            ALL_COORDINATES[coord.id].X, ALL_COORDINATES[coord.id].Y, ALL_COORDINATES[coord.id].Z,
            
            static_cast<double>(X) / static_cast<double>(N_division),

            (ALL_COORDINATES[static_cast<int>(otherDir)] - ALL_COORDINATES[static_cast<int>(coord.id)]).X,
            (ALL_COORDINATES[static_cast<int>(otherDir)] - ALL_COORDINATES[static_cast<int>(coord.id)]).Y,
            (ALL_COORDINATES[static_cast<int>(otherDir)] - ALL_COORDINATES[static_cast<int>(coord.id)]).Z,

            ret.X, ret.Y, ret.Z
        );
         */


        return ALL_COORDINATES[coord.id] +
            (static_cast<double>(X) / static_cast<double>(N_division)) * X_dir +
            (static_cast<double>(Y) / static_cast<double>(N_division)) * Y_dir;
    }
};


struct FIcosaPoint
{
    FIcosaPointCoord pointCoord;

    TArray<FVector>* hexVertices;
    TArray<FVector>* neighbourVertices;

    ~FIcosaPoint() {
        if (hexVertices != nullptr) {
            delete hexVertices;
        }
        if (neighbourVertices != nullptr) {
            delete neighbourVertices;
        }
    }

    FIcosaPoint(EIcosID id, FVector2D xy, int N_division) :
        pointCoord(id, xy, N_division)
    {
        this->hexVertices = new TArray<FVector>;
        this->neighbourVertices = new TArray<FVector>;

        this->getNeighbours();
        this->getFaceCoordinates();
    }

    FIcosaPoint(EIcosID id, int x, int y, int N_division)
        : FIcosaPoint(id, FVector2D(x, y), N_division)
    { }

    void getNeighbours() {
    /*
        *   This function computes 3D Vector coordinates of
        */

        /*
        if (this->coord.id == EIcosID::ID_NORTH_POLE) {
            this->neighbours.push_back(FIcosaPointCoord(EIcosID::ID_FIRST_FACE, 0, 7, N_division).getCoordinate());
            this->neighbours.push_back(FIcosaPointCoord(EIcosID::ID_SECOND_FACE, 0, 7, N_division).getCoordinate());
            this->neighbours.push_back(FIcosaPointCoord(EIcosID::ID_THIRD_FACE, 0, 7, N_division).getCoordinate());
            this->neighbours.push_back(FIcosaPointCoord(EIcosID::ID_FOURTH_FACE, 0, 7, N_division).getCoordinate());
            this->neighbours.push_back(FIcosaPointCoord(EIcosID::ID_FIFTH_FACE, 0, 7, N_division).getCoordinate());
        }
        if (this->coord.id == EIcosID::ID_SOUTH_POLE) {
            this->neighbours.push_back(FIcosaPointCoord(EIcosID::ID_SIXTH_FACE, 7, 0, N_division).getCoordinate());
            this->neighbours.push_back(FIcosaPointCoord(EIcosID::ID_SEVENTH_FACE, 7, 0, N_division).getCoordinate());
            this->neighbours.push_back(FIcosaPointCoord(EIcosID::ID_EIGHTH_FACE, 7, 0, N_division).getCoordinate());
            this->neighbours.push_back(FIcosaPointCoord(EIcosID::ID_NINTH_FACE, 7, 0, N_division).getCoordinate());
            this->neighbours.push_back(FIcosaPointCoord(EIcosID::ID_TENTH_FACE, 7, 0, N_division).getCoordinate());
        }
        */
        
        TArray<FIcosaPointCoord> neighbours;
        
        neighbours.Add(upNeighbour());
        auto elem = neighbours[0];
        UE_LOG(LogTemp, Warning, TEXT("neigh 2D (up): %d | %d %d | 3D: %f | %f | %f\n"), elem.coord.id, elem.X, elem.Y, elem.w_position.X, elem.w_position.Y, elem.w_position.Z);

        neighbours.Add(leftNeighbour());
        elem = neighbours[1];
        UE_LOG(LogTemp, Warning, TEXT("neigh 2D (left): %d | %d %d | 3D: %f | %f | %f\n"), elem.coord.id, elem.X, elem.Y, elem.w_position.X, elem.w_position.Y, elem.w_position.Z);


        neighbours.Add(downLeftNeighbour());
        elem = neighbours[2];
        UE_LOG(LogTemp, Warning, TEXT("neigh 2D (downLeft): %d | %d %d | 3D: %f | %f | %f\n"), elem.coord.id, elem.X, elem.Y, elem.w_position.X, elem.w_position.Y, elem.w_position.Z);



        neighbours.Add(downNeighbour());
        elem = neighbours[3];
        UE_LOG(LogTemp, Warning, TEXT("neigh 2D (down): %d | %d %d | 3D: %f | %f | %f\n"), elem.coord.id, elem.X, elem.Y, elem.w_position.X, elem.w_position.Y, elem.w_position.Z);

        neighbours.Add(rightNeighbour());
        elem = neighbours[4];
        UE_LOG(LogTemp, Warning, TEXT("neigh 2D (right): %d | %d %d | 3D: %f | %f | %f\n"), elem.coord.id, elem.X, elem.Y, elem.w_position.X, elem.w_position.Y, elem.w_position.Z);


        neighbours.Add(upRightNeighbour());
        elem = neighbours[5];
        UE_LOG(LogTemp, Warning, TEXT("neigh 2D (upRight): %d | %d %d | 3D: %f | %f | %f\n"), elem.coord.id, elem.X, elem.Y, elem.w_position.X, elem.w_position.Y, elem.w_position.Z);



         
        for (const auto& elem2 : neighbours) {
            if (elem2.coord.id == EIcosID::ID_EMPTY)
                continue;
            UE_LOG(LogTemp, Warning, TEXT("neigh 2D: %d | %d %d | 3D: %f | %f | %f\n"), elem2.coord.id, elem2.X, elem2.Y, elem2.w_position.X, elem2.w_position.Y, elem2.w_position.Z);
            this->neighbourVertices->Add(elem2.w_position);
        }

    }


    FIcosaPointCoord upNeighbour() {
        int neigh_X = this->pointCoord.X;
        int neigh_Y = this->pointCoord.Y + 1;
        EIcosID neigh_id = this->pointCoord.coord.id;
        FVector2D neigh_XY = FVector2D(neigh_X, neigh_Y);

        FIcosaVertexNeighbourEntry::correctCoordinate(this->pointCoord.coord, neigh_id, neigh_XY, this->pointCoord.N_division);

        //if (this->pointCoord.N_division == neigh_Y) {  // switch
        //    EIcosLevel neigh_level = this->pointCoord.coord.id_level;

        //    if ((neigh_level == EIcosLevel::L_SECOND_RING)) { // lower ring  (READY)
        //        neigh_id = this->pointCoord.coord.upRight();  // just go without any edge case
        //        neigh_Y = 0;
        //    }
        //    else if (neigh_level == EIcosLevel::L_FIRST_RING) { // if first ring (READY)
        //        if ((neigh_X == 0)) {  // goes to pole on x=0 axis
        //            neigh_id = this->pointCoord.coord.upRight(); // north pole
        //            neigh_Y = 0;
        //        }
        //        else { // otherwise OK (READY)
        //            neigh_id = this->pointCoord.coord.right();
        //            neigh_Y = this->pointCoord.N_division - neigh_X;
        //            neigh_X = 0;
        //        }
        //    }
        //    else {  // Poles? Maybe not...

        //    }
        //}
        return FIcosaPointCoord(neigh_id, neigh_XY, this->pointCoord.N_division);
    }
    
    FIcosaPointCoord downNeighbour() {
        int neigh_X = this->pointCoord.X;
        int neigh_Y = this->pointCoord.Y - 1;
        EIcosID neigh_id = this->pointCoord.coord.id;

        FVector2D neigh_XY = FVector2D(neigh_X, neigh_Y);

        FIcosaVertexNeighbourEntry::correctCoordinate(this->pointCoord.coord, neigh_id, neigh_XY, this->pointCoord.N_division);

        //if (-1 == neigh_Y) {
        //    EIcosLevel neigh_level = this->pointCoord.coord.id_level;
        //    //neigh_Y = N_division - neigh_X - 1;
        //    neigh_Y = 0;

        //    if (neigh_level == EIcosLevel::L_FIRST_RING)
        //        neigh_id = this->pointCoord.coord.downLeft();
        //    if (neigh_level == EIcosLevel::L_SECOND_RING)
        //        neigh_id = this->pointCoord.coord.left();

        //}

        return FIcosaPointCoord(neigh_id, neigh_XY, this->pointCoord.N_division);
    }
    FIcosaPointCoord leftNeighbour() {
        int neigh_X = this->pointCoord.X - 1;
        int neigh_Y = this->pointCoord.Y;
        EIcosID neigh_id = this->pointCoord.coord.id;

        FVector2D neigh_XY = FVector2D(neigh_X, neigh_Y);

        FIcosaVertexNeighbourEntry::correctCoordinate(this->pointCoord.coord, neigh_id, neigh_XY, this->pointCoord.N_division);

        /*UE_LOG(LogTemp, Warning, TEXT("Left neighbour(): %d | %d %d | \n"),
            static_cast<int>(neigh_id),
            neigh_X, neigh_Y);


        if (neigh_X == -1) {
            UE_LOG(LogTemp, Warning, TEXT("Corner case: %d | %d %d | \n"), 
                this->pointCoord.coord.id,
                neigh_X, neigh_Y);

            EIcosLevel neigh_level = this->pointCoord.coord.id_level;

            UE_LOG(LogTemp, Warning, TEXT("It is in level: %d \n"),
                static_cast<int>(neigh_level));


            if (neigh_level == EIcosLevel::L_SECOND_RING) {
                neigh_id = FIcosaPentCoord(neigh_id).upLeft();
            }
            if (neigh_level == EIcosLevel::L_FIRST_RING) {
                
                neigh_id = FIcosaPentCoord(neigh_id).left();

                UE_LOG(LogTemp, Warning, TEXT("It is first ring, using left neighbour IcosaFace! orig: %d -> left: %d \n"),
                    static_cast<int>(this->pointCoord.coord.id),
                    static_cast<int>(neigh_id));
            
            }
            neigh_X = this->pointCoord.N_division - neigh_Y - 1;
            neigh_Y = this->pointCoord.N_division - 1;
        }*/

        return FIcosaPointCoord(neigh_id, neigh_XY, this->pointCoord.N_division);
    }
    FIcosaPointCoord rightNeighbour() {
        int neigh_X = this->pointCoord.X + 1;
        int neigh_Y = this->pointCoord.Y;
        EIcosID neigh_id = this->pointCoord.coord.id;

        FVector2D neigh_XY = FVector2D(neigh_X, neigh_Y);

        FIcosaVertexNeighbourEntry::correctCoordinate(this->pointCoord.coord, neigh_id, neigh_XY, this->pointCoord.N_division);

        /*if (this->pointCoord.N_division == neigh_X) {
            EIcosLevel neigh_level = this->pointCoord.coord.id_level;
            if ((neigh_Y == 0) || (neigh_level == EIcosLevel::L_FIRST_RING))
                neigh_id = this->pointCoord.coord.downRight();
            else
                neigh_id = this->pointCoord.coord.right();

            neigh_X = this->pointCoord.N_division - neigh_Y;
            neigh_Y = 0;

        }*/

        return FIcosaPointCoord(neigh_id, neigh_XY, this->pointCoord.N_division);
    }
    FIcosaPointCoord upRightNeighbour() { // Hard
        int neigh_X = this->pointCoord.X + 1;
        int neigh_Y = this->pointCoord.Y + 1;


        EIcosID neigh_id = this->pointCoord.coord.id;
        /*if (this->pointCoord.N_division == neigh_Y) {
            neigh_id = this->pointCoord.coord.right();
            neigh_X = 0;
            neigh_Y = 0;
        }*/

        FVector2D neigh_XY = FVector2D(neigh_X, neigh_Y);

        FIcosaVertexNeighbourEntry::correctCoordinate(this->pointCoord.coord, neigh_id, neigh_XY, this->pointCoord.N_division);

        return FIcosaPointCoord(neigh_id, neigh_XY, this->pointCoord.N_division);
    }
    FIcosaPointCoord downLeftNeighbour() { // Hard
        int neigh_X = this->pointCoord.X - 1;
        int neigh_Y = this->pointCoord.Y - 1;
        
        
        //if (0 == neigh_Y) {
        //    // IDK
        //}
        //// Pentagon edge case
        /*if ((neigh_X == -1) && (neigh_Y == -1)) {
            return FIcosaPointCoord(EIcosID::ID_EMPTY, neigh_X, neigh_Y, this->pointCoord.N_division);
        }*/

        EIcosID neigh_id = this->pointCoord.coord.id;
        FVector2D neigh_XY = FVector2D(neigh_X, neigh_Y);

        FIcosaVertexNeighbourEntry::correctCoordinate(this->pointCoord.coord, neigh_id, neigh_XY, this->pointCoord.N_division);
        return FIcosaPointCoord(neigh_id, neigh_XY, this->pointCoord.N_division);
    }

    void getFaceCoordinates() {
        FVector& me = this->pointCoord.w_position;

        for (int i = 0; i < neighbourVertices->Num(); i++) { // need to have a specific ordering! CW!

            int next = (i + 1) % (neighbourVertices->Num());
            UE_LOG(LogTemp, Warning, TEXT("i: %d | next: %d\n"), i, next);

            FVector& elem = (*neighbourVertices)[i];
            FVector& elem_new = (*neighbourVertices)[next];


            hexVertices->Add((elem + me + elem_new) / 3.0); // betweem point
            //hexVertices->Add(elem);
        }
    }
};

struct FHexGridRenderData
{
    TArray<FVector> Vertices;
    TArray<FVector> IcosaVertices;
    TArray<FVector> AllChunkVertices;

    TArray<FVector> Normals;
    TArray<int32> Triangles;
    FString name;
};

/**
 * 
 */
class LOGOS_API SphereIcosaMeshGenerator
{
public:
    SphereIcosaMeshGenerator(uint32_t radius, uint32_t N_divisions) :
        RADIUS(radius), N_DIVISIONS(N_divisions)
    {
        UE_LOG(LogTemp, Warning, TEXT("Radius: %d | N_divisions %d\n"), radius, N_divisions);
    }

    ~SphereIcosaMeshGenerator();

    FHexGridRenderData generateHexagon(int faceID, int x, int y)
    {
        FHexGridRenderData ret;

        
        UE_LOG(LogTemp, Warning, TEXT("face: %d | x: %d | y: %d\n"), faceID, x, y);
        
        FIcosaPoint hexObject(static_cast<EIcosID>(faceID), x, y, this->N_DIVISIONS);
        
        auto hexVertices = hexObject.hexVertices;

        FVector center = scaleToRadius(hexObject.pointCoord.w_position);
        ret.Vertices.Add(center);
        //ret.Vertices.Add(hexObject.pointCoord.w_position); // TODO remove
        //ret.Normals.Add(center.GetSafeNormal());

        for (int i = 0; i < hexVertices->Num(); i++)
        {
            FVector neigh = scaleToRadius((*hexVertices)[i]);
            ret.Vertices.Add(neigh);
            //ret.Vertices.Add((*hexVertices)[i]);// TODO remove

            //ret.Normals.Add(neigh.GetSafeNormal());
            UE_LOG(LogTemp, Warning, TEXT("X: %d | Y: %d | Z: %d\n"), faceID, x, y);
            ret.Triangles.Add(0);  // center
            ret.Triangles.Add(i + 1); // this vertex
            ret.Triangles.Add(((i + 2) + (hexVertices->Num() - 1)) % hexVertices->Num() + 1); // prev vertex
        }

        // TODO clear
        for (int i = 0; i < 12; i++) {
            ret.IcosaVertices.Add(ALL_COORDINATES[i].GetSafeNormal() * this->RADIUS);
        }

       /* ret.IcosaVertices.Add(ALL_COORDINATES[0].GetSafeNormal() * this->RADIUS);
        ret.IcosaVertices.Add(ALL_COORDINATES[1].GetSafeNormal() * this->RADIUS);
        ret.IcosaVertices.Add(ALL_COORDINATES[2].GetSafeNormal() * this->RADIUS);
        ret.IcosaVertices.Add(ALL_COORDINATES[5].GetSafeNormal() * this->RADIUS);
        ret.IcosaVertices.Add(ALL_COORDINATES[6].GetSafeNormal() * this->RADIUS);
        ret.IcosaVertices.Add(ALL_COORDINATES[7].GetSafeNormal() * this->RADIUS);*/


        //ret.Vertices.Add(FIcosaPointCoord(ID_FIRST_FACE, 0, 0, this->N_DIVISIONS).w_position.GetSafeNormal() * this->RADIUS);
        //ret.Vertices.Add(FIcosaPointCoord(ID_NORTH_POLE, 0, 0, this->N_DIVISIONS).w_position.GetSafeNormal() * this->RADIUS);
        //ret.Vertices.Add(FIcosaPointCoord(ID_SECOND_FACE, 0, 0, this->N_DIVISIONS).w_position.GetSafeNormal() * this->RADIUS);
        

        for (int xi = 0; xi < 4; xi++) {
            for (int yi = 0; yi < 4; yi++) {
                ret.AllChunkVertices.Add(FIcosaPointCoord(ID_FIRST_FACE, xi, yi, this->N_DIVISIONS).w_position.GetSafeNormal() * this->RADIUS);
                ret.AllChunkVertices.Add(FIcosaPointCoord(ID_FIFTH_FACE, xi, yi, this->N_DIVISIONS).w_position.GetSafeNormal() * this->RADIUS);
            }
        }


        ret.name = FString::Printf(TEXT("%d_%d_%d"),
            static_cast<int32>(faceID), x, y);

        return ret;
    }

    FVector scaleToRadius(const FVector& icosaVec)
    {
        return icosaVec.GetSafeNormal() * this->RADIUS;
    }


protected:
    double RADIUS;
    uint32_t N_DIVISIONS;
};
