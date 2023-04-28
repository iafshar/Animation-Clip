class DOF
{
private:
    float value;
    float min;
    float max;
public:
    void setValue(float v);
    float getValue();
    void setMin(float mi);
    float getMin();
    void setMax(float ma);
    float getMax();
    void update(float change);
};
