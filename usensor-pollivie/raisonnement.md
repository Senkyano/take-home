# Code embarque

Generalement on fait un typage strict comme on ne sait pas sur quel architecture on seras

pour recevoir des informations generalement dans la majorite on seras en little endian donc le plus petit octet seras envoye en premier donc reconstituer du plus haut aux plus bas avec des decalage d'octet vers la gauche

Pour le choix de l'architecture je ne connaissais pas trop de choix d'architecture pour ce cas mais avec des recherches sur des architecture multi-thread je suis tombe sur codevault producer-consumer et j'ai implementer celui si avec une queue circulaire qui me semblait le plus judicieux.


