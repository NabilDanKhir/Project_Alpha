#ifndef POINTS_H
#define POINTS_H

class Points {
    private:
        int points;
    public:
        Points();
        void addPoints(int pts);
        int getPoints() const;
        void resetPoints();
};

#endif // POINTS_H