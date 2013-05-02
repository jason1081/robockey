// PHOTOTRANSISTOR: DOM

ISR(ADC_vect) {
    switch (port) {
        case 0:
            clear(ADMUX, MUX2); // This is the front right. Range 50-1023. Port is F1
            clear(ADMUX, MUX1);
            set(ADMUX, MUX0);
            photo5 = ADC; // For some reason, this is actually setting the value of side right
            port = 1;
            ADC = 0;
            break;
        case 1:
            set(ADMUX, MUX2); // This is the side right. Range 379-950. Port is F4
            clear(ADMUX, MUX1);
            clear(ADMUX, MUX0); // This is the side right. Range 350-950. Port is F4
            photo1 = ADC; // For some reason, this is actually setting the value of front right
            port = 2;
            ADC = 0;
            break;
        case 2:
            set(ADMUX, MUX2);
            clear(ADMUX, MUX1);
            set(ADMUX, MUX0); // This is the side left. Range 150-950. Port is F5
            photo2 = ADC;
            ADC = 0;
            port = 3;
            break;
        case 3:
            set(ADMUX, MUX1); // This is the front left side. Port is F6
            set(ADMUX, MUX2);
            clear(ADMUX, MUX0);
            photo3 = ADC;
            ADC = 0;
            port = 4;
            break;
        case 4:
            set(ADMUX, MUX1); // This is the possesion one. Port is F7
            set(ADMUX, MUX2);
            set(ADMUX, MUX0);
            photo4 = ADC;
            ADC = 0;
            port = 0;
    }
    set(ADCSRA, ADSC);
}


// PHOTOTRANSISTOR: STEPH

ISR(ADC_vect) {
    switch (port) {
        case 0:
            clear(ADMUX, MUX2); // This is the front right. Range 50-1023. Port is F1
            clear(ADMUX, MUX1);
            set(ADMUX, MUX0);
            photo1 = ADC; // For some reason, this is actually setting the value of side right
            port = 1;
            ADC = 0;
            break;
        case 1:
            set(ADMUX, MUX2); // This is the side right. Range 379-950. Port is F4
            clear(ADMUX, MUX1);
            clear(ADMUX, MUX0); // This is the side right. Range 350-950. Port is F4
            photo5 = ADC; // For some reason, this is actually setting the value of front right
            port = 2;
            ADC = 0;
            break;
        case 2:
            set(ADMUX, MUX2);
            clear(ADMUX, MUX1);
            set(ADMUX, MUX0); // This is the side left. Range 150-950. Port is F5
            photo4 = ADC;
            ADC = 0;
            port = 3;
            break;
        case 3:
            set(ADMUX, MUX1); // This is the front left side. Port is F6
            set(ADMUX, MUX2);
            clear(ADMUX, MUX0);
            photo3 = ADC;
            ADC = 0;
            port = 4;
            break;
        case 4:
            set(ADMUX, MUX1); // This is the possesion one. Port is F7
            set(ADMUX, MUX2);
            set(ADMUX, MUX0);
            photo2 = ADC;
            ADC = 0;
            port = 0;
    }
    set(ADCSRA, ADSC);
}
