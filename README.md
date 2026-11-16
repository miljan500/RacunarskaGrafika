# Računarska grafika – Laboratorijske vežbe
## Tehnologija 
**GDI** (Graphics Device Interface) je Windows grafički API koji pomoću jezika C++ omogućava aplikacijama iscrtavanje 2D grafike (linija, oblika, teksta i bitmapa) na ekranu nezavisno od samog hardvera.
## Lab1
Projekat iscrtava **2D geometrijsku scenu (tangram)** korišćenjem osnovnih GDI primitiva, različitih **četki (brushes)**, **šrafura (hatch patterns)** i **koordinatne mreže**. Pritiskom na taster G crta se mreža (**grid**).
<br/><br/>
<img width="1269" height="493" alt="image" src="https://github.com/user-attachments/assets/ea848f67-ae14-48cf-949e-183c0a888673" />
</br>
## Lab2 
Projekat crta figuru **kaktusa** (kombinacijom primitiva i GDI metafile-ova) i omogućava **interaktivnu rotaciju** delova kaktusa koji su označeni svetlijom bojom pomoću **svetskih transformacija**. 
<br/><br/>
<img width="1406" height="494" alt="image" src="https://github.com/user-attachments/assets/b40cc7e8-0611-4523-b31e-c391a079c5de" />
<br/>
## Lab3
Projekat sklapa slagalicu (**Mona Lisa**) korišćenjem svetskih transformacija za pozicioniranje pojedinačnih delova.  
<br/>
Pored uklanjanja pozadine (**transparentnosti**) i primene filtera nijansi sive (**grayscale**) direktno na nivou piksela bitmapa, implementirana je i anti-flicker tehnika (**double buffering**) za glatko pomeranje plavog dela slagalice pomoću tastera sa strelicama.
<br/><br/>
Pojedinačni delovi (**ulazne bitmape**):
<br/>
<img width="1307" height="451" alt="image" src="https://github.com/user-attachments/assets/ec5b5a9d-cd83-41eb-a6fe-abac0833731b" />
<br/><br/>
Sklopljena slagalica (**rezultat**):
<br/><br/>
<img width="517" height="612" alt="image" src="https://github.com/user-attachments/assets/674019dc-68e2-444b-9f2e-746fed24da68" />
<br/>
