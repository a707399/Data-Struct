#include "mixSpanTree.h"

void init_graph(GraphMtx* gm){
  gm->MaxVertices = Default_vertex_size;
  gm->NumEdges = gm->NumVertices = 0;

  //kai pi ding dian de nei cun kong jian
  gm->VerticesList = (T*)malloc(sizeof(T) * (gm->MaxVertices));
  assert(NULL != gm->VerticesList);

  //创建二维数组
  //让一个int的二级指针，指向一个有8个int一级指针的数组
  //开辟一个能存放gm->MaxVertices个int一级指针的内存空间
  gm->Edge = (int**)malloc(sizeof(int*) * (gm->MaxVertices));
  assert(NULL != gm->Edge);
  //开辟gm->MaxVertices组，能存放gm->MaxVertices个int的内存空间
  for(int i = 0; i < gm->MaxVertices; ++i){
    gm->Edge[i] = (int*)malloc(sizeof(int) * gm->MaxVertices);
  }
  //初始化二维数组
  //让每个顶点之间的边的关系都为不相连的
  for(int i = 0; i < gm->MaxVertices; ++i){
    for(int j = 0; j < gm->MaxVertices; ++j){
      if(i == j)
	gm->Edge[i][j] = 0;
      else
	gm->Edge[i][j] = MAX_COST;
    }
  }
}
//打印二维数组
void show_graph(GraphMtx* gm){
  printf("  ");
  for(int i = 0; i < gm->NumVertices; ++i){
    printf("%c  ", gm->VerticesList[i]);
  }
  printf("\n");
  for(int i = 0; i < gm->NumVertices; ++i){
    //在行首,打印出顶点的名字
    printf("%c:", gm->VerticesList[i]);
    for(int j = 0; j < gm->NumVertices; ++j){
      if(gm->Edge[i][j] == MAX_COST){
	printf("%c  ", '*');
      }
      else{
	printf("%d  ", gm->Edge[i][j]);
      }
    }
    printf("\n");
  }
  printf("\n");
}
//插入顶点
void insert_vertex(GraphMtx* gm, T v){
  //顶点空间已满，不能再插入顶点了
  if(gm->NumVertices >= gm->MaxVertices){
    return;
  }
  gm->VerticesList[gm->NumVertices++] = v;
}

int getVertexIndex(GraphMtx* gm, T v){
  for(int i = 0; i < gm->NumVertices; ++i){
    if(gm->VerticesList[i] == v)return i;
  }
  return -1;
}
//添加顶点间的线
void insert_edge(GraphMtx* gm, T v1, T v2, E cost){
  if(v1 == v2)return;
  
  //查找2个顶点的下标
  int j = getVertexIndex(gm, v1);
  int k = getVertexIndex(gm, v2);
  //说明找到顶点了,并且点之间还没有线
  if(j != -1 && k != -1 ){
    //因为是无方向，所以更新2个值
    gm->Edge[j][k] = gm->Edge[k][j] = cost;
    //边数加一
    gm->NumEdges++;
  }
}
//删除顶点间的线
void remove_edge(GraphMtx* gm, T v1, T v2){
  if(v1 == v2)return;
  //查找2个顶点的下标
  int j = getVertexIndex(gm, v1);
  int k = getVertexIndex(gm, v2);
  //说明找到顶点了,并且点之间还有线
  if(j != -1 && k != -1 && gm->Edge[j][k] == 1){
    //因为是无方向，所以更新2个值
    gm->Edge[j][k] = gm->Edge[k][j] = 0;
    //边数减一
    gm->NumEdges--;
  }
}
//删除顶点
void remove_vertex(GraphMtx* gm, T v){
  int k = getVertexIndex(gm, v);
  if(-1 == k)return;
  
  //算出和要删除节点相关的边的数量，并减少。
  for(int i = 0; i < gm->NumVertices; ++i){
    if(gm->Edge[k][i] == 1){
      gm->NumEdges--;
    }
  }

  //如果要删除的顶点不是最后一个顶点
  if(k != gm->NumVertices - 1){
    //把每一列向左移动一列
    for(int i = 0; i < gm->NumVertices; ++i){
      //把后面内存里的内容移动到前面，并把最后一个元素设置成0
      memmove(&(gm->Edge[i][k]), &(gm->Edge[i][k+1]), sizeof(int) * (gm->NumVertices-1-k));
      gm->Edge[i][gm->NumVertices - 1] = 0;
    }
    //把每一行向上移动一行
    for(int i = k; i < gm->NumVertices - 1; ++i){
      memmove(gm->Edge[i], gm->Edge[i+1], sizeof(int) * (gm->NumVertices-1));
    }
    memset(gm->Edge[gm->NumVertices - 1], 0, sizeof(int) * (gm->NumVertices - 1));
    //memmove(&(gm->Edge[k]), &(gm->Edge[k+1]), sizeof(int*) * (gm->NumVertices-1-k));
    //memset(gm->Edge[gm->NumVertices - 1], 0, sizeof(int) * (gm->NumVertices - 1));

    //删除点
    memmove(&(gm->VerticesList[k]), &(gm->VerticesList[k+1]), sizeof(T) * (gm->NumVertices-1-k));
  }
  //如果要删除的顶点是最后一个顶点
  else{
    for(int i = 0; i < gm->NumVertices; ++i){
      gm->Edge[i][k] = gm->Edge[k][i] = 0;
    }
  }

  //节点数目减1
  gm->NumVertices--;
}

//摧毁图
void destroy_graph(GraphMtx* gm){
  free(gm->VerticesList);
  for(int i = 0; i < gm->NumVertices; ++i){
    free(gm->Edge[i]);
  }
  free(gm->Edge);
  gm->Edge = NULL;
  gm->VerticesList = NULL;
  gm->MaxVertices = gm->NumVertices = gm->NumEdges = 0;
}

//取得与某顶点有连线的第一个顶点
int getNeighbor(GraphMtx* gm, T v){
  int p = getVertexIndex(gm, v);
  if(-1 == p)return -1;
  for(int i = 0; i < gm->NumVertices; ++i){
    if(gm->Edge[p][i] == 1)
      return i;
  }
  return -1;
}

//取得与v1顶点，v1顶点之后的v2顶点的之后的有连线的第一个顶点
int getNextNeighbor(GraphMtx* gm, T v1, T v2){
  if(v1 == v2)return -1;
  int p1 = getVertexIndex(gm, v1);
  int p2 = getVertexIndex(gm, v2);
  if(p1 == -1 || p2 == -1)return -1;

  for(int i = p2 + 1; i < gm->NumVertices; ++i){
    if(gm->Edge[p1][i] == 1)
      return i;
  }

  return -1;
}

//取得2个顶点间的权重
E getWeight(GraphMtx* g, int i1, int i2){
  if(i1 == -1 || i2 == -1)
    return MAX_COST;
  else
    return g->Edge[i1][i2];
}
//用prim算法作成最小树
void minSpanTree_prim(GraphMtx* g, T v){
  int n = g->NumVertices;
  E* lowcost = (E*)malloc(sizeof(E) * n);
  int* mst = (int*)malloc(sizeof(int) * n);
  assert(lowcost != NULL && mst != NULL);

  int k = getVertexIndex(g, v);

  for(int i = 0; i < n; ++i){
    if(i != k){
      lowcost[i] = getWeight(g, k, i);
      mst[i] = k;
    }
    else{
      lowcost[i] = 0;
    }
  }

  int min, min_index;
  int begin, end;
  E cost;
  for(int i = 0; i < n - 1; ++i){
    min = MAX_COST;
    min_index = -1;
    for(int j = 0; j < n; ++j){
      if(lowcost[j] != 0 && lowcost[j] < min){
	min = lowcost[j];
	min_index = j;
      }
    }
    begin = mst[min_index];
    end = min_index;
    printf("%c->%c:%d\n",g->VerticesList[begin],g->VerticesList[end],min);

    lowcost[min_index] = 0;

    for(int j = 0; j < n; ++j){
      cost = getWeight(g, min_index, j);
      if(cost < lowcost[j]){
      //if(lowcost[j] != 0){
	lowcost[j] = cost;
	mst[j] = min_index;
      }
    }
  }

}
