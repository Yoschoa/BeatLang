# BeatLang : Conception et Implémentation d'un Langage Dédié (DSL) au Séquençage Rythmique

## 1. Introduction et Objectifs Académiques
**BeatLang** est un langage de programmation dédié (Domain-Specific Language - DSL) conçu pour la modélisation, l'analyse et la synthèse de séquences polyrythmiques. Ce projet repose sur l'implémentation complète d'un compilateur en **C++ moderne (C++17)**.

L'objectif pédagogique et technique de ce projet est d'illustrer la théorie de la compilation en construisant une chaîne de traitement complète (pipeline) : de l'analyse lexicale du code source jusqu'à l'exécution et la génération d'un flux audio multithread en temps réel. Il met en application des concepts avancés d'ingénierie logicielle tels que la gestion déterministe de la mémoire (RAII), les pointeurs intelligents (`std::unique_ptr`), et le patron de conception (Design Pattern) **Visitor**.

---

## 2. Architecture Logicielle et Pipeline de Compilation

Le compilateur BeatLang adopte une architecture modulaire stricte, isolant la représentation des données des algorithmes de traitement.

### 2.1. Front-End (Analyse Lexicale et Syntaxique)
* **Scanner (Lexer) :** Procède à la tokénisation du flux de caractères entrant. Il identifie les lexèmes valides et écarte les commentaires ou caractères d'espacement.
* **Parser :** Implémente un analyseur syntaxique par descente récursive (Recursive Descent Parser). Il valide la structure du programme par rapport à la grammaire formelle et génère une représentation intermédiaire sous la forme d'un **Arbre Syntaxique Abstrait (AST)**.

### 2.2. Middle-End (Analyse Sémantique)
* **Semantic Analyzer :** S'appuie sur le patron Visitor pour effectuer un parcours en profondeur de l'AST. Il réalise l'analyse contextuelle, le typage statique des séquences (vérification des contraintes de caractères de vélocité) et lève des exceptions typées en cas d'incohérence logique, garantissant ainsi la sûreté de l'exécution.

### 2.3. Back-End (Moteur de Rendu Audio)
* **Audio Engine :** Encapsulation de l'API bas niveau `miniaudio`. Ce module charge les ressources binaires (`.wav`) directement en mémoire vive (RAM) lors de l'initialisation.
* **Audio Visitor :** Parcours final de l'AST effectuant les calculs temporels (conversion des BPM en intervalles de millisecondes) et déclenchant les appels système asynchrones vers le contrôleur audio de l'hôte.

---

## 3. Spécification Syntaxique et Grammaticale formelle

Le code source BeatLang utilise l'extension `.bl`. La syntaxe a été élaborée pour maximiser la lisibilité des matrices rythmiques.

### Exemple de script BeatLang :
```beatlang
tempo 120;

pattern funk_groove {
    kick:       "X...X.x.X...X...";
    snare:      "....X.......X...";
    hihat:      "xXxXxXxXxXxXxXxX";
}

song {
    loop 4 {
        play funk_groove;
    }
}
```
(Lexique des quantifications : `X` = Accent/Vélocité max, `x` = Ghost Note/Vélocité réduite, `.` = Silence, `|` = Séparateur visuel).

### Grammaire Simplifiée (Pseudo-EBNF) :
```text
Program     -> TempoDecl PatternDecl+ SongDecl
TempoDecl   -> "tempo" NUMBER ";"
PatternDecl -> "pattern" IDENTIFIER "{" TrackDecl+ "}"
TrackDecl   -> IDENTIFIER ":" STRING ";"
SongDecl    -> "song" "{" Statement+ "}"
Statement   -> PlayStmt | LoopStmt
PlayStmt    -> "play" IDENTIFIER ";"
LoopStmt    -> "loop" NUMBER "{" Statement+ "}"
```

---

## 4. Modélisation Mathématique du Séquençage Temporel

Le cœur de l'Audio Engine repose sur un calcul déterministe du temps. Pour synchroniser le multithreading avec précision, le visiteur audio (Audio Visitor) transforme les métriques musicales (BPM, Signatures Rythmiques) en valeurs temporelles absolues (millisecondes).

### 4.1. Résolution Temporelle d'une Étape (Step Duration)
Dans l'architecture actuelle de BeatLang, la grille temporelle (Grid Resolution) est définie sur la **double croche** (16th note). Chaque caractère de la séquence (par exemple `X` ou `.`) représente l'une de ces étapes.

Pour calculer la durée physique d'une étape, nous appliquons l'équation suivante :
1. Conversion de la minute en millisecondes : $1\text{ minute} = 60000\text{ ms}$.
2. Calcul de la durée d'une noire (1 Battement) : $\text{Noire} = \frac{60000}{\text{BPM}}$.
3. Division par 4 (car une noire contient exactement 4 doubles croches).

La formule mathématique simplifiée intégrée au compilateur est donc :
$$\text{Durée de l'Étape (ms)} = \frac{60000}{\text{BPM}} \div 4 = \frac{15000}{\text{BPM}}$$

### 4.2. Impact de la Signature Rythmique sur l'Allocation Mémoire
Bien que la durée absolue d'une étape reste constante (pour un BPM donné), la **Signature Rythmique** (Time Signature) dicte la longueur totale de la chaîne de caractères allouée à une mesure. 

Une signature rythmique est un rapport mathématique où le **Numérateur** représente le nombre de temps par mesure, et le **Dénominateur** indique l'unité de temps (4 = noire, 8 = croche). Le calcul du nombre total d'étapes (caractères) par mesure s'effectue via cette équation :
$$\text{Total Étapes} = \text{Numérateur} \times \left( \frac{16}{\text{Dénominateur}} \right)$$

**Exemples d'application :**
* Métrique standard (4/4) : $4 \times (16 \div 4) = 16\text{ étapes}$ (Ex: `"X...X...X...X..."`)
* Métrique asymétrique (7/8) : $7 \times (16 \div 8) = 14\text{ étapes}$ (Ex: `"X.x.X.x.X.x.X."`)
* Métrique ternaire (6/8) : $6 \times (16 \div 8) = 12\text{ étapes}$ (Ex: `"X.xX.xX.xX.x"`)

---

## 5. Perspectives et Évolutions Futures (BeatLang V2.0)

L'architecture de l'AST et du système de Visitors a été pensée pour être hautement extensible. Les travaux futurs se concentreront sur l'enrichissement de la grammaire pour supporter une sémantique musicale et temporelle plus complexe.

### 5.1. Modélisation de Signatures Rythmiques Complexes
Implémentation formelle de la directive `timeSignature` pour contraindre structurellement les métriques asymétriques (ex: 7/8, 5/4) lors de l'analyse sémantique.
```beatlang
tempo 120;
timeSignature 7/8;
```
Impact architectural : Le `SemanticAnalyzer` devra vérifier dynamiquement la longueur des chaînes de caractères (`String`) des pistes pour s'assurer qu'elles correspondent exactement à la formule mathématique induite par le numérateur et le dénominateur de la signature (voir section 4.2).

### 5.2. Contraintes de Borne Temporelle Globale
Ajout d'une directive de contrainte temporelle définissant la durée exacte de la composition.
```beatlang
songDuration 4m; // Définition de la durée à 4 minutes
```
Impact architectural : L'analyseur sémantique ou le moteur audio devra calculer le nombre d'itérations nécessaires du bloc `song` pour combler ou tronquer la durée spécifiée, modifiant ainsi l'arbre d'exécution au moment du Runtime.

### 5.3. Opérateurs de Résolution et Contexte d'Exécution (Dot Operator)
Introduction d'un paradigme orienté objet pour les appels de fonctions via l'opérateur point (`.`). Cela permettra d'appliquer des mutateurs locaux à un Pattern lors de son invocation.
```beatlang
song {
    // Joue le pattern pendant exactement 30 secondes
    play pattern_funk.duration(30s); 
    
    // Joue le pattern en altérant son tempo localement
    play pattern_trap.tempo(140);
}
```
Impact architectural : Le `Parser` devra être mis à jour pour reconnaître ces *Tokens* et générer des `PlayNode` enrichis. Le visiteur audio appliquera alors ces états locaux (surcharge de paramètres) au lieu de lire les constantes globales de l'AST.

---

## 6. Instructions de Déploiement

### Prérequis Système
* Environnement POSIX (Linux, macOS) ou WSL.
* Compilateur `g++` conforme au standard **C++17**.
* Outil de build `make`.

### Compilation (Build)
Le projet utilise un `Makefile` optimisant la compilation séparée (le moteur audio lourd n'est compilé qu'une seule fois).
```bash
# Compilation multi-cœurs (fortement recommandée)
make -j8 beatlang
```

### Exécution (Runtime)
Passage du fichier de script en argument au binaire généré :
```bash
./bin/beatlang scripts/mon_rythme.bl
```