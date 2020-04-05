#define nbColumns 7
#define nbLines 5
#define maxi 35

const int columns[nbColumns] = { 5, 10, 9, 6, 0, 2, 3 };
const int lines[nbLines]      = { 8, 13, 12, 4, 1 };

void setup()
{
    for(int n=0; n < nbColumns; n++)
    {
        pinMode(columns[n],INPUT);
    }
    for(int n=0; n < nbLines; n++)
    {
        pinMode(lines[n],OUTPUT);
        digitalWrite(lines[n],LOW);
    }
    Serial.begin(115200);
    Serial.setTimeout(10);
}

void loop()
{
    if(Serial.available() > 0)
    {
        Serial.readString();
        Serial.flush();
        for(int l = 0; l < nbLines; l++)
        {
            digitalWrite(lines[l],HIGH);
            Serial.print("LINE ");
            Serial.print(l);
            Serial.println(" : HIGH");
            for(int c = 0; c < nbColumns; c++)
            {
                while(!Serial.available() > 0)
                {
                    
                }
                if(Serial.available() > 0)
                {
                    Serial.readString();
                    Serial.flush();
                    bool b = digitalRead(columns[c]);
                    if(b)
                    {
                        Serial.print("COLUMN ");
                        Serial.print(c);
                        Serial.println(" : HIGH !!!");
                    }
                    else
                    {
                        Serial.print("COLUMN ");
                        Serial.print(c);
                        Serial.println(" : LOW");
                    }
                }
            }
            digitalWrite(lines[l],LOW);
            Serial.print("LINE ");
            Serial.print(l);
            Serial.println(" : LOW");
        }      
    }
}
