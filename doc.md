# Halite Bot Documentation

## Coding Standard

|                             | Standard 1     | Standard 2       |
|-----------------------------|----------------|------------------|
| Class name                  | ClassName      | ClassName        |
| Variable name               | variableName   | variable_name    |
| Ptr variable name           | pVariableName  | p_variable_name  |
| Ref variable name           | rVariableName  | r_variable_name  |
| Member variable name        | mVariableName  | m_variable_name  |
| Static member variable name | sVariableName  | s_varaible_name  |
| Const variable name         | VARIABLE_NAME  | VARIABLE_NAME    |
| Function name               | FunctionName   | function_name    |
| Combined (ptr+member)       | mpVariableName | mp_variable_name |
| Enum field                  | EnumField      | enum_field       |
| namespace name              | NamespaceName  | namespace_name   |

## Program Architecture

### Behaviour tree

One per ship (each chip can make one move per turn)
One for the shipyard ?


## IA ideas

Trouver les points d'interêts (POI)
Rusher les POI de notre côté dangereux (x > 20) avec 2 vaisseaux
Rusher les POI safe avec un vaisseau
Poser un dropoff sur le point de valeur du POI
Commencer à pomper sur le 2eme point de valeur du POI
Récolter l'halite
Passer à la case suivante si 2 tour sur la case actuelle rapportent moins que 1 sur la suivante

Un ennemi se présente devant notre POI :
    - Toujours être en superiorité numérique
    - Regarder si il est sur une case de valeur du POI
