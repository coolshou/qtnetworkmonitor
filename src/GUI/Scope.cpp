#include <QtGui>

#include "Scope.h"

Scope::Scope(QWidget *parent)
 : QWidget(parent)
{
    debug = 0;

    //Update the screen
    //QTimer *timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    //timer->start(1000);

    setWindowTitle(tr("Scope"));
    resize(600, 200);
    setMinimumSize(60,30);
}

void Scope::Set_Data(vector<float> data_in, int target)
{
    assert( target >= 0 );
    assert( target < (int)data.size() +1 );

    //The data about to be pushed needs to be intialized in the vector
    if( target == static_cast<int>(data.size()))
    {
        data.push_back( data_in );
        vector<float> newData;
        Scale.push_back( newData );
    }

    data[target].assign(data_in.begin(), data_in.end());
    Scale[target] = Find_Scale(data_in);
}
/*
float abs(float Fin)
{
    if (Fin < 0)
        return -Fin;
    else
        return Fin;
}
*/
vector<float> Scope::Find_Scale(vector<float> in_Data)
{
    vector<float> Scale(4);

    if ( in_Data.size() == 0)
    {
        //cout<<"WARNING: No in_Data to scale in Scope::Find_Scale()."<<endl;
        in_Data.push_back(0);
    }

    //assert(in_Data.size()>0);

    float max_y = in_Data[0];
    float min_y = in_Data[0];

    for (unsigned int x=0; x < in_Data.size(); x++)
    {
        if ( (in_Data[x]) > (max_y))
            max_y = (in_Data[x]);

        if ( (in_Data[x]) < (min_y))
            min_y = (in_Data[x]);
    }

    Scale[0] = 0;
    Scale[1] = in_Data.size()-1;
    Scale[2] = min_y;
    Scale[3] = max_y;

    //Avoid unpleasant states
    if (Scale[0] == Scale[1])
    {
        //cout<<"WARNING: Problem setting the scale_x."<<endl;
        Scale[0] = 0.0;
        Scale[1] = 1.0;
    }

    if (Scale[1] == Scale[2])
    {
        //cout<<"WARNING: Problem setting the scale_y."<<endl;
        Scale[2] = 0.0;
        Scale[3] = 1.0;
    }

    return Scale;
}

void Scope::paintEvent(QPaintEvent *)
{
    //Set the painting device
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    //painter.save();

    //Draws a line around the scope
    painter.setPen(Qt::black);
    painter.translate(0, 0);
    painter.drawLine(0,0 , width(),0);
    painter.drawLine(width(),0 , width(), height());
    painter.drawLine(width(), height() , 0, height());
    painter.drawLine(0, height() , 0,0);

    for (unsigned int data_x = 0; data_x < data.size(); data_x++)
    {
        painter.resetMatrix();

        int Scale_to_use = 0;
        int MaxEmphY=0;
        //Finds the 'biggest' scale
        for (unsigned int i=0; i<Scale.size(); i++)
        {
            if ( (Scale[i][3] - Scale[i][2]) > MaxEmphY)
            {
                MaxEmphY = static_cast<int>((Scale[i][3] - Scale[i][2]));
                Scale_to_use = i;
            }

        }

        if (data[data_x].size() > 1)
        {
            //Scale so that the data is spread on the entire area of the widget
            float x_denom = abs(Scale[Scale_to_use][0]) + abs(Scale[Scale_to_use][1]);
            float y_denom = abs(Scale[Scale_to_use][2]) + abs(Scale[Scale_to_use][3]);

            //assert(x_denom!=0 && y_denom!=0);

            float scale_x = width()/(x_denom);
            float scale_y = height()/(y_denom);

            float distanca_from_x_axis = 0;
            if ( Scale[data_x][2] < 0)
                distanca_from_x_axis = Scale[data_x][2];

            painter.translate( 0, height() + distanca_from_x_axis*scale_y );

            //Draws the data
            if (data_x==0)
                painter.setPen(Qt::green);
            else if (data_x==1)
                painter.setPen(Qt::red);
            else if (data_x==2)
                painter.setPen(Qt::yellow);
            else
                painter.setPen(Qt::black);

            //cout<<"-------------"<<data_x<<endl;
            for (unsigned int x= 1; x < data[data_x].size(); x++)
            {
                //Data_y is reversed to fit an intuitive coordinate system
                painter.drawLine( int((x-1)*scale_x), int(-data[data_x][x-1]*scale_y), int(x*scale_x), int(-data[data_x][x]*scale_y) );
            }

        }//If data...
    }
}
