# Automaatne-kaablirull

Mehatroonikasüsteemide õppeaine projekt: automaatse tagasikeritava pikendusjuhtme juhtprogramm Arduino C++ keeles.

## Programm
[Kaablirull.ino](Kaablirull/Kaablirull.ino)

## Algoritmi plokkskeem

```mermaid
flowchart TD
 A([Algus]) --> B[1. Lähtestamine]
 B --> C[2. Impulsside lugemine]
 C --> D[3. Pikkuse arvutamine]
 D --> E[4. Pikkuse kuvamine]
 E --> F{5. Nupp vajutatud?}
 F -- Ei --> C
 F -- Jah --> G{6. Kaablit on väljas?}
 G -- Jah --> H[7. Mootori käivitamine]
 H --> I[8. Pikkuse uuendamine]
 I --> J{9. Kaabel tagasi keritud?}
 J -- Ei --> I
 J -- Jah --> K[10. Mootori peatamine]
 G -- Ei --> K
 K --> L([Ooterežiim])
 L --> C
```

## Komponendid ja eeldused

Arduino Uno, enkooder D2/D3, nupp D4, lõppasendi lüliti D7, mootoridraiver D8, I2C LCD SDA/SCL. Vajalik LiquidCrystal_I2C teek.

Näidisväärtus 5 mm impulsi kohta tuleb kalibreerida. Enkoodri suund tuleb kontrollida. Käivitamisel eeldatakse nullasendit. Lisatud on 30 sekundi ajalimiit ja lõppasendi lüliti. Mootorit ei ühendata otse Arduino väljundisse. Koodi ei ole füüsilisel seadmel testitud.
