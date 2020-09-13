#include "include/graph/graphrepresentation.h"

GraphRepresentation::GraphRepresentation()
{

}

GraphRepresentationManager::GraphRepresentationManager()
{

}

void GraphRepresentationManager::calculationIdEnd(bool status)
{
    emit calculationEnded(calculationId, status);
}
