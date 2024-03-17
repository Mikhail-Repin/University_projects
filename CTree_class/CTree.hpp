#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;
void gen_random(char *s, const int len);
void printcommands();
template <class T> class CTree
{
public:
	class CNode
	{
		T v;
		string data;
		int h, bal;
		CNode *par, *left, *right;
		public:
			CNode() {par=left=right=NULL; h=1; bal=0;}
			CNode(CNode *par, const T &v, string data) {this->par = par; this->v = v; this->data = data; left=right=NULL; h=1; bal=0;}
			CNode(const CNode&b){CopyOnly();}
			void CopyOnly(const CNode&b)
			{
				if(&b!=NULL)
				{
					this->v=b.v;			
					this->data=data; this->h=b.h;  this->bal=b.bal;
					this->par=b.par; this->left=b.left; this->right=b.right;
				}
			}
			bool IsLeft() const {if(par==NULL) {return false;} else{return par->left==this;}}
			bool IsRight() const {if(par==NULL) {return false;} else{return par->right==this;}}
			string Dat() const {return data;}
			int balance() const {return bal;}
                        int height() const {return h;}
			const T& operator*() const {return v;}
			//operator const T&() const {return v;}
			friend class CTree<T>;
	};
	class iterator
	{
		const CNode *cur; CTree *t;
		public:
			iterator() {cur=NULL; t=NULL;}
			iterator(CTree *t, const CNode *cur) {this->cur=cur; this->t=t;}
			const T& operator*() {return cur->v;}
			string Dat() {return cur->data;}
			int balance() {return cur->bal;}
			int height() {return cur->h;}
			iterator operator++() {cur=t->FindNext(cur); return *this;}
			iterator operator--() {cur=t->FindPrev(cur); return *this;}
			bool operator==(const iterator &b) {return cur==b.cur;}
			bool operator!=(const iterator &b) {return cur!=b.cur;}
			friend class CTree<T>;
	};
private:
	CNode *root; size_t n;
public:
	CTree() {SetZero();}
	void SetZero() {root = NULL; n = 0;}
	~CTree() {Clean();}
	void Clean() {while(remove(root));}
	bool remove(const CNode *n);
	bool remove(const T&v) {const CNode *n=Find(v); return remove(n);}
	bool IsEmpty() const {return root == NULL;}
	CNode* GetRoot() const {return root;}
	bool insert (const T&v, string data);
	size_t size() {return n;}
	bool Union(CNode*root2, CNode*v, size_t n2);
	void NewHeight(CNode *n) 
	{
		for(CNode*cur=n;cur->par!=NULL;cur=cur->par)
		{
			if((cur->IsLeft()&&cur->par->right==NULL) || (cur->IsRight()&&cur->par->left==NULL)) {cur->par->h=cur->h+1;}
			else if(cur->par->right!=NULL&&cur->par->left!=NULL) 
			{
				if(cur->IsLeft()) {cur->par->h=max(cur->h, cur->par->right->h)+1;}
				else {cur->par->h=max(cur->h, cur->par->left->h)+1;}
			}
		}
	}
	void NewBal(CNode *n)
	{
		for(CNode*cur=n;cur->par!=NULL;cur=cur->par)
		{
			if(cur->IsLeft())
			{
				if(cur->par->right!=NULL){cur->par->bal=cur->h-cur->par->right->h;}
				else {cur->par->bal=cur->h;}
			}
			else if(cur->IsRight())
			{
	                	if(cur->par->left!=NULL){cur->par->bal=cur->par->left->h-cur->h;}
				else {cur->par->bal=-cur->h;}	
			}
		}
	}
	void L(CNode*n) {CNode*tmp=n->par->par; int log; if(n->par->IsLeft()) {log=1;} else if(n->par->IsRight()){log=0;}n->par->right=n->left; if(n->left!=NULL){n->left->par=n->par;} n->par->par=n;n->left=n->par;n->par=tmp;if(n->par==NULL){root=n;} else{if(log){tmp->left=n;}else if(log==0){tmp->right=n;}}}
	void R(CNode*n) {CNode*tmp=n->par->par; int log; if(n->par->IsLeft()) {log=1;} else if(n->par->IsRight()){log=0;}n->par->left=n->right;if(n->right!=NULL){n->right->par=n->par;} n->par->par=n;n->right=n->par;n->par=tmp;if(n->par==NULL){root=n;} else{if(log){tmp->left=n;}else if(log==0){tmp->right=n;}}} 
	void balancing(CNode*tmp);
	const CNode* FindMin() const {if(IsEmpty()) return NULL; const CNode *n; for(n=root; n->left!=NULL; n = n->left); return n;}
	const CNode* FindMin(const CNode* root) const {if(root == NULL) return NULL; const CNode *n; for(n=root; n->left!=NULL; n = n->left); return n;}
	const CNode* FindMax() const {if(IsEmpty()) return NULL; const CNode *n; for(n=root; n->right!=NULL; n = n->right); return n;}
	const CNode* FindMax(const CNode* root) const {if(root == NULL) return NULL; const CNode *n; for(n=root; n->right!=NULL; n = n->right); return n;}
	const CNode* Find(const T&v) const {const CNode *n; for(n=root;n!=NULL;) {if((*(*n))<v) {n=n->right;} else if((*(*n))>v) {n=n->left;} else return n;} return n;}
	const CNode* FindNext(const CNode* n) const 
	{
		if(IsEmpty()) return NULL;
		if(n->right!=NULL) return FindMin(n->right);
		while(n->par)
		{
			if(n->IsLeft()) return n->par;
			n=n->par;
		}
		return NULL;
	}
	const CNode* FindPrev(const CNode* n) const 
	{
		if(IsEmpty()) return NULL;
		if(n->left!=NULL) return FindMax(n->left);
		while(n->par)
		{
			if(n->IsRight()) return n->par;
			n=n->par;
		}
		return NULL;
	}
	iterator begin() {return iterator(this, FindMin());}
	iterator end() {return iterator(this, NULL);}
	void print(CNode *n, int l)
	{
		if(n!=NULL)
		{
			print(n->right, l+1);
			for(int i=1; i<=l; i++) cout<<"      ";
			cout<<(*(*n))<<"("<<n->height()<<" "<<n->balance()<<")"<<"::"<<n->Dat()<<endl<<endl;
			print(n->left, l+1);
		}
	}
};

template <class T> bool CTree<T>::Union(CNode*root2, CNode*v, size_t n2)
{
	if(root2==NULL) return false;
	if(Find(v->v)!=NULL) return false;
	if(root->h > root2->h)
	{
		for(CNode*cur=root; cur!=NULL; cur=cur->right)
		{
			if(abs(cur->h-root2->h)==0 || abs(cur->h-root2->h)==1)
			{
				v->left=cur; v->right=root2;
				if(cur!=root) {cur->par->right=v; v->par=cur->par;}
				else {root=v;}
				cur->par=v; root2->par=v;
				(this->n)=(this->n)+n2+1;
				NewHeight(cur); NewBal(cur);
				balancing(cur); break;
			}
		}
	}
	else
	{
		for(CNode*cur=root2; cur!=NULL; cur=cur->left)
		{
			if(abs(cur->h-root->h)==0 || abs(cur->h-root->h)==1)
			{
				v->right=cur; v->left=root;
				if(cur!=root2) {cur->par->left=v; v->par=cur->par;}
				else root2=v;
				cur->par=v; root->par=v;
				(this->n)=(this->n)+n2+1;
				root=root2;
				NewHeight(cur); NewBal(cur);
				balancing(cur); break;
			}
		}
	}
return true;
}

template <class T> void CTree<T>::balancing(CNode*tmp)
{
	if(tmp==NULL) return;
	for(CNode*n=tmp; n->par!=NULL; n=n->par)
	{
		if(n->par->bal>1)
		{
			n=n->par->left;
			if(n->bal>-1)
			{
				R(n); 
				if(n->right->left==NULL && n->right->right==NULL) {n->right->h=1; n->right->bal=0; NewHeight(n->right); NewBal(n->right);}
				else if(n->right->left!=NULL) {NewHeight(n->right->left); NewBal(n->right->left);}
				else {NewHeight(n->right->right); NewBal(n->right->right);}
				if(n==root) break;
			}
			else
			{
				L(n->right);
				if(n->left==NULL && n->right==NULL) {n->h=1; n->bal=0; NewHeight(n); NewBal(n);}
				else if(n->left!=NULL) {NewHeight(n->left); NewBal(n->left);}
				else {NewHeight(n->right); NewBal(n->right);}
				n=n->par;
				R(n);
				if(n->right->left==NULL && n->right->right==NULL) {n->right->h=1; n->right->bal=0; NewHeight(n->right); NewBal(n->right);}
				else if(n->right->left!=NULL) {NewHeight(n->right->left); NewBal(n->right->left);}
				else {NewHeight(n->right->right); NewBal(n->right->right);}
				if(n==root) break;	
			}
		}
		else if(n->par->bal<-1)
		{
			n=n->par->right;
			if(n->bal==-1)
			{
				L(n);
				if(n->left->left==NULL && n->left->right==NULL) {n->left->h=1; n->left->bal=0; NewHeight(n->left); NewBal(n->left);}
				else if(n->left->left!=NULL) {NewHeight(n->left->left); NewBal(n->left->left);}
				else {NewHeight(n->left->right); NewBal(n->left->right);}
				if(n==root) break;				
			}
			else
			{
				R(n->left);                                                                                                                  
				if(n->left==NULL && n->right==NULL) {n->h=1; n->bal=0; NewHeight(n); NewBal(n);}
				else if(n->left!=NULL) {NewHeight(n->left); NewBal(n->left);}
				else {NewHeight(n->right); NewBal(n->right);}
				n=n->par;
				L(n);
				if(n->left->left==NULL && n->left->right==NULL) {n->left->h=1; n->left->bal=0; NewHeight(n->left); NewBal(n->left);}
				else if(n->left->left!=NULL) {NewHeight(n->left->left); NewBal(n->left->left);}
				else {NewHeight(n->left->right); NewBal(n->left->right);}
				if(n==root) break;
			}
		}
	}
}

template <class T> bool CTree<T>::insert(const T&v, string data)
{
	if(IsEmpty()) {root = new CNode(NULL, v, data); ++n; return true;}
	CNode *tmp;
	tmp = NULL;
	for(CNode *cur = root;;)
	{
		if((*(*cur))==v) return false;
		else if((*(*cur))<v)
		{
			if(cur->right==NULL) {cur->right=new CNode(cur, v, data); ++n; tmp=cur->right; NewHeight((const_cast<CNode*>(tmp))); NewBal((const_cast<CNode*>(tmp)));break;}
			else {cur=cur->right;}
		}
		else
		{
			if(cur->left==NULL) {cur->left=new CNode(cur, v, data); ++n; tmp=cur->left;NewHeight((const_cast<CNode*>(tmp))); NewBal((const_cast<CNode*>(tmp)));break;}
			else {cur=cur->left;}
		}
	}
	balancing(tmp); 
return true;
}

template <class T> bool CTree<T>::remove(const CNode *n)
{
	const CNode *tmp;
	tmp=NULL;
	if(root==NULL || n==NULL) return false;
	if(n->right==NULL&&n->left==NULL)
	{ 
		if(n->IsLeft()) 
		{
			n->par->left=NULL;
			if(n->par->right!=NULL) {tmp=n->par->right;}
			else {(const_cast<CNode*>(n->par))->h--; (const_cast<CNode*>(n->par))->bal--; tmp=n->par;} 
			delete n; n = NULL;
		}
		else if(n->IsRight()) 
		{
			n->par->right=NULL;
			if(n->par->left!=NULL) {tmp=n->par->left;}
			else {(const_cast<CNode*>(n->par))->h--; (const_cast<CNode*>(n->par))->bal++; tmp=n->par;} 
			delete n; n = NULL;
		}
		else {root=NULL; delete n; n=NULL; (this->n)--; return true;}
		NewHeight((const_cast<CNode*>(tmp))); NewBal((const_cast<CNode*>(tmp)));
		(this->n)--;
	}
	else if(n->left==NULL)
	{
		if(n->IsLeft()) {n->par->left=n->right; n->right->par=n->par; tmp=n->right; delete n; n = NULL;}
		else if(n->IsRight()) {n->par->right=n->right; n->right->par=n->par; tmp=n->right; delete n; n = NULL;}
		else {root=n->right; n->right->par=NULL; delete n; n = NULL;(this->n)--; return true;}
		NewHeight((const_cast<CNode*>(tmp))); NewBal((const_cast<CNode*>(tmp)));
		(this->n)--;
	}
	else if(n->right==NULL)
	{
		if(n->IsLeft()) {n->par->left=n->left; n->left->par=n->par; tmp=n->left; delete n; n = NULL;}
		else if(n->IsRight()) {n->par->right=n->left; n->left->par=n->par; tmp=n->left; delete n; n = NULL;}
		else {root=n->left; n->left->par=NULL;delete n; n = NULL;(this->n)--; return true;}
		NewHeight((const_cast<CNode*>(tmp))); NewBal((const_cast<CNode*>(tmp)));
		(this->n)--;
	}
	else
	{
		const CNode *m = FindMin(n->right);
		(const_cast<CNode*>(n))->v = m->v;
		(const_cast<CNode*>(n))->data = m->data;
		remove(m);
	}
	balancing((const_cast<CNode*>(tmp)));
return true;	
}
                                                                                                                              