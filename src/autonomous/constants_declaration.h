const int distMax = 600;  //Distância máxima permitida como referência para os sensores (em milímetros)
const int distAtk = 400;
const int distAtkMax = 200;

const int speedMax = 85; //vel max para ser usada durande o ataque
const int speedStandard = 30; //vel padrão --- Cuidado se abaixar para menos de 30!!!
const int searchSpeed = speedStandard*0.6;//vel de girar no proprio eixo
const int aproxSpeed = speedStandard*0.25;//vel ao se aproximar do oponente

int contadorFlutuacoes = 0;
