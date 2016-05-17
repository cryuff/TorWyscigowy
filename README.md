# TorWyscigowy

Użytkownik podaje ilość drużyn oraz ilość okrążeń.

Pojazdy startują, mają losową ilość paliwa z określonego zakresu.
Im mniej paliwa tym szybciej porusza się pojazd.
Poniżej określonej ilości paliwa pojazd próbuje zjechać do pitstopu.
Sekcjami krytycznymi są pitstopy oraz dwa zakręty na torze, przez które może przejechać tylko jeden pojazd.
Dodatkową sekcją krytyczną jest punkt odpoczynku dla ludzi pracujących w pitstopie.
Udają się do niego w celu regeneracji siły.
Im więcej siły posiadają pracownicy pitstopu, tym krótszy czas tankowania pojazdów w pitstopie.
Wątki - rysowanie toru wraz z pozycjami pojazdów, zespół w pitstopie, pojazdy.
Wygrywa pierwszy pojazd, który przejedzie określoną ilość okrążeń.
Pojazdy mogą nie dotrzeć do mety jeśli skończy im się paliwo, a nie będą mogły zjechać do pitstopu, gdyż drugi pojazd będzie go zajmował.
Program przetestowany do 50 pojazdów (25 zespołów) dla 500 okrążeń.
