#include "graph.h"


graph::graph(int v){
  vertex = v;
  degree = (int*)malloc(sizeof(int)*v);
  visited = (int*)malloc(sizeof(int)*v);
  bpAux = new list<int>[v];
  bfs = new list<int>[v];
  bp = new queue<int>;
  for(int i=0;i<v;i++){
    degree[i]=0;
    visited[i]=0;
  }
}

void graph::addEdge(int to,int from){
  adjMtx.push_back(make_pair(to,from));
}

void graph::showEdges(){
  int vectorSize = adjMtx.size();
  for(int i=0;i<vectorSize;i++){
    cout<<adjMtx[i].first<<"-"<<adjMtx[i].second<<endl;
  }
}

void graph::calcEdgeDegree(){
  for(int i=0;i<vertex;i++){
    degree[i]=0;
  }
  int vectorSize = adjMtx.size();
  for(int i=0;i<vectorSize;i++){
    degree[adjMtx[i].first]+=1;
    degree[adjMtx[i].second]+=1;
  }
}

void graph::showDegree(){
  calcEdgeDegree();
  for(int i=0;i<vertex;i++){
    cout<<i<<":"<<degree[i]<<endl;
  }
}

void graph::runBFS(int v){
  int vectorSize = adjMtx.size();
  queue<int> thatFila;
  for(int i=0;i<vectorSize;i++){
    bfs[adjMtx[i].first].push_back(adjMtx[i].second);
  }
  list<int>::iterator itBFS;
  visited[v]=1;
  while(true){
    for(itBFS=bfs[v].begin();itBFS!=bfs[v].end();itBFS++){
      if(visited[*itBFS]==0){
          visited[*itBFS] = 1;
          thatFila.push(*itBFS);
      }
    }
    if(!thatFila.empty()){
      v=thatFila.front();
      thatFila.pop();
    }else{
      break;
    }
  }
}

void graph::showVisited(){
  runBFS(0);
  for(int i=0;i<vertex;i++){
    cout<<i<<"-"<<visited[i]<<endl;
  }
}
int graph::testConnected(int calc,int start){
  runBFS(start);
  if(calc == 1){
    calcEdgeDegree();
  }
  for(int i=0;i<vertex;i++){
    if((degree[i]>0)&&(visited[i]==0)){
            return 0;
    }
  }
    return 1;
}

int graph::testBridge(int to,int from){
  int i;
  int vectorSize = adjMtx.size();
  for(i=0;i<vectorSize;i++){
    if((adjMtx[i].first == to)&&(adjMtx[i].second == from)){
      break;
    }
  }
  //calcEdgeDegree();//remove when test best path.
  adjMtx.erase(adjMtx.begin()+i);
  if(testConnected(0,to)==0){
    //cout<<"ponte"<<endl;
    adjMtx.push_back(make_pair(to,from));
    return 1;
  }else{
  //  cout<<"so aresta"<<endl;
    adjMtx.push_back(make_pair(to,from));
    return 0;
  }

}
void graph::getEdgesFromFile(string archiveName){
  ifstream file(archiveName.c_str());
  if(!file.is_open()){
    cout<<"erro ao abrir arquivo"<<endl;
    return;
  }
  char c;
  string fullnumber = "";
  int to = -1;
  int from = -1;
  while((file.good())&&(file.get(c))){
    if((to == -1)&&(isdigit(c))){
      fullnumber = c;
    }else if(c==':'){
      istringstream(fullnumber)>>to;
      fullnumber="";
    }
    if((to != -1)&&(isdigit(c))){
      fullnumber += c;
    }
    if((to != -1)&&((c==' ')||(c=='\n'))&&(fullnumber!="")){
      istringstream(fullnumber)>>from;
      adjMtx.push_back(make_pair(to,from));
      fullnumber="";
    }
    if((c =='\n')&&(fullnumber=="")){
      to = -1;
      from = -1;
    }
  }
}
int graph::testAllEven(){
    for(int i=0;i<vertex;i++){
      if(degree[i]%2!=0){
        return 0;
      }
    }
    return 1;
}
void graph::deleteEdge(int to, int from){
  vector<pair<int,int> >::iterator itDelete;
  for(itDelete=adjMtx.begin();itDelete!=adjMtx.end();itDelete++){
    if((itDelete->first==to)&&(itDelete->second==from)){
      adjMtx.erase(itDelete);
      return;
    }
  }
  calcEdgeDegree();
}


void graph::makeListBp(){
  vector<pair<int,int> >::iterator itbpAux;
  for(int i=0;i<vertex;i++){
    bpAux[i].clear();
  }
  for(itbpAux=adjMtx.begin();itbpAux!=adjMtx.end();itbpAux++){
    bpAux[itbpAux->first].push_back(itbpAux->second);
  }
}

void graph::invertEdge(int v){
  vector<pair<int,int> >::iterator it;
  for(it=adjMtx.begin();it!=adjMtx.end();it++){
    if(it->second == v){
      it->second = it->first;
      it->first = v;
    }
  }
}

void graph::bpAuxRec(int start,int finalStart){
  calcEdgeDegree();
  int degrees=0;
  for(int i=0;i<vertex;i++){
    degrees += degree[i];
  }
  bp->push(start);
  if((degrees==0)&&(start==finalStart)){
    cout<<"Finished With Sucess"<<endl;
    return;
  }else{
    invertEdge(start);
    makeListBp();
    list<int>::iterator itbpList;
    /*for(int i=0;i<vertex;i++){
      for(itbpList=bpAux[i].begin();itbpList!=bpAux[i].end();itbpList++){
          cout<<i<<"-"<<*itbpList<<endl;
        }
      }*/
      int thisDegree = degree[start];
      int next = -1;
      if(thisDegree > 1){
        for(itbpList=bpAux[start].begin();itbpList!=bpAux[start].end();itbpList++){
          if((next == -1)&&(testBridge(start,*itbpList)==0)){
            next = *itbpList;
          }else if((degree[next]<degree[*itbpList])&&(testBridge(start,*itbpList)==0)){
            next = *itbpList;
          }
        }
        deleteEdge(start,next);
        bpAuxRec(next,finalStart);
      }else if(thisDegree == 1){
        itbpList = bpAux[start].begin();
        next = *itbpList;
        deleteEdge(start,next);
        bpAuxRec(next,finalStart);
      }else{
        return;
      }
  }
}

void graph::runBestPath(int start){
  cout<<"iniciando pre testes"<<endl;
  if(testConnected(1,0)==0){
    cout<<"O Grafo precisa ser conexo para o BEST PATH funcionar!"<<endl;
    return;
  }
  if(testAllEven()==0){
    cout<<"Todos os Vertices precisam ter grau para o BEST PATH funcionar!"<<endl;
    return;
  }
  cout<<"pre testes concluidos..."<<endl;
  cout<<"INICIANDO"<<endl;
  bpAuxRec(start,start);
}

void graph::showBP(){
  queue<int> aux;
  int v;
  if(bp->empty()){
    runBestPath(0);
    while(!bp->empty()){
      v = bp->front();
      aux.push(v);
      bp->pop();
    }
    while(!aux.empty()){
      v = aux.front();
      cout<<v<<"-";
      bp->push(v);
      aux.pop();
    }
    cout<<"FIM"<<endl;
  }else{
    while(!bp->empty()){
      v = bp->front();
      aux.push(v);
      bp->pop();
    }
    while(!aux.empty()){
      v = aux.front();
      cout<<v<<"-";
      bp->push(v);
      aux.pop();
    }
    cout<<"FIM"<<endl;
  }
}
