#include<iostream>
#include<vector>
#include<unordered_map>
#include<queue>
#include<fstream>
#include<sstream>
using namespace std;
class Node
{
	char ch;
	int freq;
	Node *LNode, *RNode;
	public:
    Node(char ch,int freq,Node*LNode,Node*RNode)
	{
		this->ch = ch;
		this->freq = freq;
		this->LNode = LNode;
		this->RNode = RNode;
    }
	friend class comp;
	friend Node* Design_huffman_tree(string);
	friend string Dcode(string, Node*);
	friend void Ncode(Node*,vector<bool>&str,unordered_map<char,vector<bool>>&Hcode);
};
class comp
{
	public:
	bool operator()(Node* l, Node* r)
	{
		return l->freq > r->freq;
	}
};
Node* Design_huffman_tree(string text)
{
	/*
	unordered_map<char,int> to store the frequency of each character present in the string text
	*/
	unordered_map<char, int> freq;
	for(char ch: text)
	{
      freq[ch]++;
	}
	priority_queue<Node*, vector<Node*>, comp> pq;
	/*
	Minimum Prioirty_Queue to maintain 
	        -->> most frequently used letter have the shortest encoding and vice-versa <<--
	*/
	for (auto pair: freq)
	{
		Node*ptr = new Node(pair.first, pair.second, NULL, NULL);
		pq.push(ptr);
	}

   while(pq.size() != 1)
   {
   	Node*left = pq.top();pq.pop();
   	Node*right = pq.top();pq.pop();
   	pq.push(new Node('\0',left->freq+right->freq,left,right));
   }
   return pq.top()/* root Node */;
}
void Ncode(Node* root, vector<bool>&str,unordered_map<char, vector<bool>> &HCode)
{
	if (!root)
		return;
	/*
	    storing the paths from root to leaf of each characters, and mapping into a STL unordered_map
		The Idea Used is Simple:
		    if we are traversing to left from current Node we will append 0, else we will append 1
	*/
	if (!root->LNode && !root->RNode) 
	{
		/*
		Mapping the value in unordered_map for leaf->ch
		*/
		HCode[root->ch] = str;
		return;
	}
	str.push_back(1);
	Ncode(root->LNode, str, HCode);
	str.pop_back();
	str.push_back(0);
	Ncode(root->RNode, str, HCode);
	str.pop_back();
}
string Dcode(string to_decode,Node*root)
{
	/* 
	   Decoding : we have to pointers (IthIndex, temp) and a pointer to the root varialbe.
	   while, Traversing the to_decode binary string if to_decode[IthIndex] == 0, we will move the pointer temp
	   its left child's Node. Once we reech the leaf node we will store the character present in the Node and
	   move the temp pointer to root, and so on..................
	                                    (root)
									  0 /    \ 1
									   /      \
								   (LNode)  (RNode)
	*/
	Node*temp = root;
	int IthIndex = 0;
	string decoded_str = "";
	int d_size = to_decode.size();
	while(IthIndex <= d_size)
	{
      if(!temp->RNode && !temp->LNode)
	  {
		decoded_str = decoded_str + temp->ch;
		temp = root;
	  }else if(to_decode[IthIndex] == '1')
	  {
		temp = temp->LNode;
		IthIndex++;
	  }else
	  {
		temp = temp->RNode;
		IthIndex++;
	  }
	}
	return decoded_str;
}
int main()
{
    /* Importing .txt file and storing into f */
    ifstream file("Original.txt");

	/* Storing the imported .txt file into string text */
	string text;
	if(file)
	{
		ostringstream ss;
		ss << file.rdbuf();
		text = ss.str();
	}
	file.close();
	/* Building of Huffman Tree */
	Node*root = Design_huffman_tree(text);

	/*......................***....................***ENCODING***.......................***..................*/

	/* Encoding the each characters present in the string to s sequence of unique Binary Numbers */
	vector<bool>str;
	unordered_map<char,vector<bool>>Hcode;
	Ncode(root,str,Hcode);
    /* for(auto x : Hcode){
		cout<<x.first<<" ";
		for(auto x : x.second)cout<<x;cout<<endl;
	} */
	/* Storing the String in terms of Binary digits with the help of Encoded data to a newly created EncodedFile.txt file */
	ofstream MyFile("Compressed.txt");
    for(int i=0;i<text.size();i++){
		for(auto x : Hcode[text[i]]){
			MyFile << x;
		}
    }
    MyFile.close();
	/* 

	 The Above EncodedFile.txt file is a text file of Binary data type which represents Compressed File
	
	*/

    /*......................***....................***DECODING***.......................***..................*/

	/* Extrating the EncodedFile.txt which was created on the above steps & storing it into to_encode string */
	ifstream file_("Compressed.txt");
	string to_decode;
	if(file_)
	{
		ostringstream ss;
		ss << file_.rdbuf();
		to_decode = ss.str();
	}
	file_.close();
	string decoded_str = Dcode(to_decode,root);
	ofstream MyDecodedFile("Decoded_File.txt");
    for(auto x : decoded_str)
	{
		MyDecodedFile << x;
	}
	MyDecodedFile.close();
	return 0;
}