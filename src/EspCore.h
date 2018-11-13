#ifndef ESP_CORE_H
#define ESP_CORE_H

class EspCore
{

    public:

        void connectWifi();

        void autoConnect();

        void sleep(int minutes);

};

#endif // ESP_CORE_H