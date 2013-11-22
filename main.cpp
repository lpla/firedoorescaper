#include "include/cgame.h"
#include "include/cfiredoor.h"
#include "GodMachine.h"
#include <iostream>
#include <vector>

using namespace FireDoorEscaper;

//TODO: Métodos de clasificación a implementar
//Fáciles
// - Lineal (Perceptron, Regresión Lineal, etc)
// - Lineal con transoformaciones no lineales

//Medios
// - Regresión logística
// - Redes neuronales
//     - Feed-Forward ( Backpropagation)
//     - Recurrentes / Generales (Genéticos u otros) <- Medio-difícil

//Difíciles
// - Support Vector Machines
//     - Kernel Methods (Difícil++)
// - Radial Basis Functions  (Difícil--)


//TODO: Técnicas de clasficiación a implementar
// - Regularización <- Medio
// - Reducción de dimensionalidad
//      - Recomendado: PCA <- Medio-difícil
//      - Alternativo: Aprendizaje no-supervisado (k-means, etc)


//Niveles para la nota de la práctica:
//·Básico: 2 modelos (fácil y media como mínimo)
//         1 técnica
// Nota razonable: 3 modelos (Medio y difícil)
//                 1-2 técnicas


// Print Game Status Values
void
printGameStatus(const CGame& g) {
    const CFireDoor& fd = g.getCurrentFireDoor();
    const CFireDoor::TVecDoubles& inp = fd.getNextStepInputs();
    std::cout   << "Lvl: " << g.getLevel()
                << " | Steps: " << g.getSteps()
                << " | Score: " << g.getScore()
                << " | Door: " << g.getDoorsPassed() + 1
                << " | DSteps: " << fd.getSteps()
                << " | OnFire?: " << ((fd.isOnFire()) ? "yes" : "no")
                << std::endl
                << "=========================================================================\n"
                << "Next Inputs: [ ";
    for (unsigned i=0; i < inp.size(); i++)
        std::cout << inp[i] << " ";
    std::cout << "] \n";
}

int
main(void) {
    // Create a new game starting at level 0, and staying at the same level all the time.
    // Use GDM_LEVELUP for increasing level of difficulty
	GodMachine machine;
    CGame *game = new CGame(0);
    game->setGameDifficultyMode(CGame::GDM_SAMELEVEL);
    Sample s;


    // Main loop: stay will the game is on (i.e. the player is alive)
    std::cout << "Empezamos el juego: " << std::endl;
    while (game->getGameStatus() == CGame::GS_PLAYING) {
    	machine.clearTrainingSet();

        const CFireDoor& fd = game->getCurrentFireDoor();
        printGameStatus(*game);

        //Almacenamos el conjunto de entrenamiento
    	while(!machine.isTrainingReady()){
        //for (unsigned i=0; i < 5; i++) {
            printGameStatus(*game);
            s.input=fd.getNextStepInputs();
            game->nextStep();
            s.burn=fd.isOnFire();
            machine.addTrainingSample(s);
        }
    	//Hago esto para obtener un vector de doubles de los input
    	std::vector<double> vIn = fd.getNextStepInputs();
    	double* in = &vIn[0];
    	while(!machine.isReadyToCross() || machine.isDoorOnFire(in)){
    		printGameStatus(*game);
            s.input=fd.getNextStepInputs();
            game->nextStep();
            s.burn=fd.isOnFire();
            machine.classifySample(s);
            machine.addTrainingSample(s);
    	}

        // Try to cross the current FireDoor
        printGameStatus(*game);
        std::cout << "**** TRYING TO CROSS THE DOOR ****\n";
        game->crossFireDoor();
        if (game->getGameStatus() != CGame::GS_PLAYING)
            std::cout << "!!!!!!!!!!! PLAYER GOT BURNED OUT !!!!!!!!!!!!!!\n";
        else
            std::cout << "****** DOOR PASSED *****\n";

    }

    // Game Over
    delete game;
    return 0;
}
