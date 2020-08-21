#include "include/graph/lgraphrepresentation.h"
#include "include/poset.h"

#include <QCoreApplication>
#include <QGraphicsSimpleTextItem>
#include <QMessageBox>
#include <QProgressDialog>

std::size_t factorial(std::size_t n)
{
    return n>1 ? n*factorial(n-1) : 1;
}

/*!
 * \brief rankPermutation
 * The algorithm to rank permutations as formulated by Martin Mares in http://mj.ucw.cz/papers/rank.ps
 * The implementation isn't completely linear, but is at most O(n^2)
 * \param permutation
 * \param i
 * \param n
 * \return
 */
std::size_t rankPermutation(std::vector<vertex> permutation, std::size_t i, std::size_t n)
{
    if (i >= n)
        return 0;

    std::size_t a = 0;
    for(std::size_t j = i+1; j < n; ++j)
    {
        if(permutation[j] < permutation[i])
            a++;
    }

    return a * factorial(n-i-1) + rankPermutation(permutation, i+1, n);
}

LGraphQThread::LGraphQThread(QObject *parent) : QThread(parent)
{

}

LGraphQThread::~LGraphQThread()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();
    wait();
}


void LGraphQThread::check(std::vector<vertex>& x_vertices, std::vector<vertex>& y_vertices, std::map<vertex, std::set<vertex>> &inedges, std::size_t newId)
{
    mutex.lock();
    id = newId;
    vertices_x = x_vertices;
    vertices_y = y_vertices;
    std::sort(std::next(vertices_y.begin(), 1), std::prev(vertices_y.end(), 1));
    edges = inedges;
    mutex.unlock();

    if (!isRunning())
        start(LowPriority);
    else
        condition.wakeOne();
}

void LGraphQThread::run()
{
    forever
    {
        reprMutex.lock();
        mutex.lock();
        if(abort)
        {
            reprMutex.unlock();
            mutex.unlock();
            return;
        }
        bool result = repr.checkBFPermutations(vertices_x, vertices_y, edges);
        reprMutex.unlock();
        emit finishedCalculation(result, id);
        condition.wait(&mutex);
        mutex.unlock();
    }
}


LGraphRepresentationManager::LGraphRepresentationManager()
{

}

void LGraphRepresentationManager::generateFromGraph(Graph& graph)
{
    return generateFromGraphBF(graph);
}

/*!
 * \brief LGraphRepresentationManager::generate_from_graph_BF
 * Attempt to create the representation by checking all possible
 * (n!)^2 permutations
 * \param graph The graph which is to be represented
 */
void LGraphRepresentationManager::generateFromGraphBF(Graph& graph)
{
    std::vector<vertex> vertices_x = graph.getVertices();
    std::vector<vertex> vertices_y = graph.getVertices();

    return generateFromGraphStateBF(graph, vertices_x, vertices_y);
}

void LGraphRepresentationManager::generateFromGraphStateBF(Graph& graph, std::vector<vertex> vertices_x, std::vector<vertex> vertices_y)
{
    edges = graph.getEdges();

    viable = false;
    aborted = false;
    stopped = 0;
    xVertices = vertices_x;
    yVertices = vertices_y;

    std::size_t size = vertices_x.size();

    // Large graphs aren't permitted yet
    if(size > 20)
    {
        // 21! > 2^64, although I would estimate that at most 11-12 is feasible
        // to be run in a decent timeframe
        emit graphTooBig();
        return;
    }

    permutations = factorial(size);
    permutationsPerTick = (permutations / INT_MAX) + 1;
    currentPermutation = rankPermutation(vertices_x, 0, size);
    ticks = static_cast<int>(permutations / permutationsPerTick);
    counter = 0;

    emit calculationStarted(ticks);

    // If the calculation has already been started, tick the required number of ticks
    emit calculationTick(static_cast<int>(currentPermutation/permutationsPerTick));

    int threadNo = QThread::idealThreadCount();
    threads.resize(threadNo);
    for(int i = 0; i < threadNo; i++)
    {
        threads[i] = std::unique_ptr<LGraphQThread>(new LGraphQThread());
        threads[i]->check(xVertices, xVertices, edges, i);
        connect(threads[i].get(), SIGNAL(finishedCalculation(bool, int)), this, SLOT(threadFinished(bool, int)));
        if (!std::next_permutation(xVertices.begin(), xVertices.end()))
        {
            break;
        }
    }
}

void LGraphRepresentationManager::draw(QGraphicsView& view)
{
    threads[foundSolution]->draw(view);
}

void LGraphRepresentationManager::threadFinished(bool status, int id)
{
    viable = viable || status;
    if(!viable && !aborted)
    {
        if(!std::next_permutation(xVertices.begin(), xVertices.end()))
        {
            // TODO: not necessarily correct
            emit calculationFinished(ticks);
            emit calculationEnded(false);
            return;
        }
        // Update the progress dialog
        counter++;
        if(counter == permutationsPerTick)
        {
            emit calculationTick(1);
            counter = 0;
        }
        threads[id]->check(xVertices, xVertices, edges, id);

    }
    else if(viable)
    {
        if(status)
            foundSolution = id;
        stopped++;
        if(stopped == threads.size())
        {
            emit calculationFinished(ticks);
            emit calculationEnded(true);
        }
    }
    else if(aborted)
    {
        stopped++;
        if(stopped == threads.size())
        {
            int result = QMessageBox::question(nullptr, "Save the computation state?", "Do you want to save the computation state?", 0, 1);
            if(result == 1)
                emit saveState(xVertices, yVertices);
        }
    }
}

LGraphExtRepresentationManager::LGraphExtRepresentationManager()
{

}

void LGraphExtRepresentationManager::generateFromGraph(Graph& g)
{
    std::vector<vertex> vertices_x = g.getVertices();
    aborted = false;
    std::size_t size, permutations, permutationsPerTick, counter;
    int ticks;

    size = vertices_x.size();
    // Large graphs aren't permitted yet
    if(size > 18) // A quick change to check the Blanusa snark
    {
        // At most 11-12 is currently feasible due to memory constraints
        emit graphTooBig();
        return;
    }

    permutations = factorial(size);
    permutationsPerTick = (permutations / INT_MAX) + 1;
    ticks = static_cast<int>(permutations / permutationsPerTick);
    counter = 0;
    emit calculationStarted(ticks);

    do {
        // Process events so that the application doesn't stop responding
        QCoreApplication::processEvents();

        if(checkPoset(vertices_x, g))
        {
            emit calculationFinished(ticks);
            emit calculationEnded(true);
            return;
        }

        counter++;
        if(counter == permutationsPerTick)
        {
            emit calculationTick(1);
            counter = 0;
        }
    } while(std::next_permutation(vertices_x.begin(), vertices_x.end()) && !aborted);

    emit calculationFinished(ticks);
    emit calculationEnded(false);
}

void LGraphExtRepresentationManager::draw(QGraphicsView &view)
{
    repr.draw(view);
}

bool LGraphExtRepresentationManager::checkPoset(std::vector<vertex>& vertices_x, Graph &g)
{
    Poset p(vertices_x.size());
    auto edges = g.getEdges();
    for(std::size_t i = 0; i<vertices_x.size(); ++i)
    {
        for(std::size_t j=i+1; j<vertices_x.size(); ++j)
        {
            // If there is an edge, or the left vertex is the first one
            // (the first vertex on the x axis can be WLOG the first one in the y axis)
            if(edges.at(vertices_x[i]).count(vertices_x[j])>0 || i == 0)
            {
                p.addInequality(i, j);
            }
        }
    }
    std::queue<std::vector<std::size_t>> q;

    p.findAllLinearExtensions(vertices_x, q);

    bool found = false;
    std::vector<vertex> vertices_y;
    // Try all found linear extensions
    while(!(q.empty() || found))
    {
        vertices_y.clear();
        vertices_y.resize(vertices_x.size());
        auto v = q.front();
        for(std::size_t i = 0; i<v.size(); ++i)
        {
            vertices_y[i] = vertices_x[v[i]];
        }
        repr.checkBFPermutation(vertices_x, vertices_y, edges);

        if(repr.isRepresentationViable(edges))
        {
            found = true;
            result_x = vertices_x;
            result_y = vertices_y;
        }
        q.pop();
    }

    return found;
}


LGraphRepresentation::LGraphRepresentation()
{

}

bool LGraphRepresentation::checkBFPermutations(std::vector<vertex>& vertices_x, std::vector<vertex>& vertices_y, std::map<vertex, std::set<vertex>>& edges)
{
    do
    {
        checkBFPermutation(vertices_x, vertices_y, edges);
        if(isRepresentationViable(edges))
            return true;
    }
    while(std::next_permutation(vertices_y.begin()+1, vertices_y.end()));
    return false;
}

void LGraphRepresentation::checkBFPermutation(std::vector<vertex>& vertices_x, std::vector<vertex>& vertices_y, std::map<vertex, std::set<vertex>>& edges)
{
    std::map<vertex, std::pair<std::size_t, std::size_t>> coordinates;
    for(std::size_t i = 0; i < vertices_x.size(); ++i)
    {
        coordinates.insert(std::make_pair(vertices_x[i], std::make_pair(i, 0)));
    }
    for(std::size_t i = 0; i < vertices_x.size(); ++i)
    {
        coordinates.at(vertices_y[i]).second = vertices_x.size() - i - 1;
    }
    representation.clear();

    // Create the representation without any height or length at first
    for(auto it=coordinates.begin(); it != coordinates.end(); ++it)
    {
        LNode n;
        n.v = it->first;
        n.x = it->second.first;
        n.y = it->second.second;
        representation.push_back(n);
    }

    // Add the height and length based on the edges in O(n^2*log^2(n))
    // TODO: the runtime of this could be improved rather easily
    // Sort by x coordinates and calculate width by simply extending for
    // as long as possible.
    std::sort(representation.begin(), representation.end(), [](LNode a, LNode b){return a.x < b.x;});
    for(auto it=representation.begin(); it!=representation.end(); ++it)
    {
        if(it == representation.begin())
        {
            it->width = representation.size()-1;
            continue;
        }

        std::size_t current_width = 0, max_width = 0;
        // If the vertex has no edges, there is no need to extend
        if(edges.at(it->v).size() > 0)
        {
            for(auto it2=it+1; it2 != representation.end(); ++it2)
            {
                current_width++;
                // If the next node could intersect and they don't have a common
                // edge, don't continue with the extension
                if(edges.at(it->v).count(it2->v) > 0)
                {
                    max_width = current_width;
                }

            }
        }
        it->width = max_width;
    }

    // Sort by y coordinates and calculate height
    std::sort(representation.begin(), representation.end(), [](LNode a, LNode b){return a.y < b.y;});
    for(auto it=representation.begin(); it!=representation.end(); ++it)
    {
        std::size_t max_height = 0, current_height = 0;
        // If the vertex has no edges, there is no need to extend
        if(edges.at(it->v).size() > 0)
        {
            for(auto it2=it+1; it2 != representation.end(); ++it2)
            {
                current_height++;
                // If the next node's L would intersect and they don't have a common
                // edge, don't continue with the extension
                if(edges.at(it->v).count(it2->v) > 0)
                {
                    max_height = current_height;
                }
            }
        }
        it->height = max_height;
    }

}

bool LGraphRepresentation::isRepresentationViable(std::map<vertex, std::set<vertex>> edges)
{
    std::sort(representation.begin(), representation.end(), [](LNode a, LNode b){return a.v < b.v;});
    for(auto it=representation.begin(); it != representation.end(); ++it)
    {
        for(auto it2=it+1; it2 != representation.end(); ++it2)
        {
            if((LNode::intersection(*it, *it2) && edges.at(it->v).count(it2->v) == 0) || (!LNode::intersection(*it, *it2) && edges.at(it->v).count(it2->v) > 0))
            {
                return false;
            }
        }
    }

    return true;
}

void LGraphRepresentation::draw(QGraphicsView& view)
{
    QGraphicsScene* scene = view.scene();
    std::size_t node_nr = representation.size();

    scene->clear();
    QRectF rect = scene->sceneRect();

    // No vertices - there is nothing to draw
    if(node_nr == 0)
        return;

    constexpr qreal BORDER = 10;
    constexpr qreal FONT_SIZE = 10;
    qreal top = rect.top() + BORDER, left = rect.left() + BORDER;
    qreal width = view.size().width() - 2 * BORDER, height = view.size().height() - 2 * BORDER;
    qreal unit_width = width / (node_nr);
    qreal unit_height = height / (node_nr);

    // Draw the L for every node
    for (auto node: representation)
    {
        qreal node_x = node.x * unit_width + left;
        qreal node_y = (node_nr - node.y - 0.25) * unit_height + top;
        qreal node_width = (node.width + 0.75) * unit_width;
        qreal node_height = (node.height + 0.75) * unit_height;

        scene->addLine(node_x, node_y, node_x + node_width, node_y);
        scene->addLine(node_x, node_y - node_height, node_x, node_y);

        QGraphicsSimpleTextItem* text = scene->addSimpleText(QString::number(node.v));
        text->setX(node_x+1.5*FONT_SIZE);
        text->setY(node_y-2*FONT_SIZE);

    }

}
