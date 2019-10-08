#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <cctype>
#include <algorithm>
#include <boost/algorithm/string.hpp>

using namespace std;


char homepage_indicator = 'N';
std::stack<std::string>	stk_subject, stk_predicate, stk_object;
std::ofstream ofs;

std::string get_subject(string& triple);
std::string get_predicate(string& triple);
std::string get_object(string& triple);
//**********************************************************
void add_json_ld_doc_sub(std::stack<std::string>& stk_subject, std::stack<std::string>& stk_predicate, std::stack<std::string>& stk_object);
//**********************************************************
void add_json_iswc_sub(std::stack<std::string>& stk_subject, std::stack<std::string>& stk_predicate, std::stack<std::string>& stk_object);
//**********************************************************
void add_json_3triples_sub(std::stack<std::string>& stk_subject, std::stack<std::string>& stk_predicate, std::stack<std::string>& stk_object);

int main()
{
	string subject, predicate, object;


	std::ifstream in("/home/ubuntu/BSBM/tenmil.nt");
	ofs.open ("/home/ubuntu/EDBT/output/tenbsbmDL.json", std::ofstream::out | std::ofstream::trunc);
	

	
	ofs << "[\n";
	std::string  triple;

	while (std::getline(in, triple))
	{

		subject   = get_subject(triple);
		predicate = get_predicate(triple);
		if ( predicate == "\"homepage\"")
			homepage_indicator = 'Y';
		object    = get_object(triple);

		if (stk_subject.empty())
			stk_subject.push(subject);
		
		else if ( stk_subject.top() != subject)
		{
			//add_json_ld_doc_sub(stk_subject,stk_predicate,stk_object);
			//add_json_iswc_sub(stk_subject,stk_predicate,stk_object);
			add_json_3triples_sub(stk_subject,stk_predicate,stk_object);
			stk_subject.push(subject);
			
		}
		
		stk_predicate.push(predicate);
		stk_object.push(object);
	}
		if (!stk_subject.empty())				
			add_json_3triples_sub(stk_subject,stk_predicate,stk_object);
			//add_json_iswc_sub(stk_subject,stk_predicate,stk_object);
			//add_json_ld_doc_sub(stk_subject,stk_predicate,stk_object);

		ofs << "\n";
		ofs << "{}]";
		ofs.close();


return 0;
}// int main()

//******************************************************
//
//******************************************************
void add_json_3triples_sub(std::stack<std::string>& stk_subject, std::stack<std::string>& stk_predicate, std::stack<std::string>& stk_object){



	while ( stk_predicate.size() > 0)
	{
			
		ofs << "{\n";
		ofs << "\t\"Subject\":" << stk_subject.top() << ",\n";
		ofs << "\t\"Predicate\":" << stk_predicate.top() << "," ;
		ofs << "\t\"Object\": "   << stk_object.top() ;
		ofs << "},\n";
		stk_predicate.pop();
		stk_object.pop();

	}
	
	stk_subject.pop();


	

}//add_json_3triples_sub

//******************************************************
//
//******************************************************
void add_json_iswc_sub(std::stack<std::string>& stk_subject, std::stack<std::string>& stk_predicate, std::stack<std::string>& stk_object){

	
	ofs << "{\n";
	ofs << "\t\"_id\":" << stk_subject.top() << ",\n";
	stk_subject.pop();

	ofs << "\t\"Predicates \": [" ;

	while ( stk_predicate.size() > 1)
	{
		ofs << stk_predicate.top() << ",";
		stk_predicate.pop();

	}
		ofs << stk_predicate.top() << "],";
		stk_predicate.pop();

	ofs << "\t\"Objects \": [" ;

	while ( stk_object.size() > 1)
	{
		ofs << stk_object.top() << ",";
		stk_object.pop();

	}
		ofs << stk_object.top() << "]";
		stk_object.pop();

	ofs << "},\n";




}//add_json_iswc_sub

//******************************************************
//
//******************************************************
void add_json_ld_doc_sub(std::stack<std::string>& stk_subject, std::stack<std::string>& stk_predicate, std::stack<std::string>& stk_object){
	//std::ofstream ofs;
	//ofs.open ("./ISWC/iswc.json", std::ofstream::out | std::ofstream::app);
	std::string temp_top_predicate;
	ofs << "{\n";
	ofs << "\t\"_id\":" << stk_subject.top() << ",\n";
	stk_subject.pop();


	while (stk_object.size() > 1 )
	{
		if (stk_predicate.size() > 1)
		{				
			temp_top_predicate = stk_predicate.top();
			stk_predicate.pop();
		}

		if ( !(stk_predicate.top() == temp_top_predicate))
		{
			ofs << "\t" << temp_top_predicate << ":" << stk_object.top() << ",\n";
			stk_object.pop();
		}else
		{
			
			ofs << "\t" << temp_top_predicate << ":[";
			ofs <<  stk_object.top() << ",";
	 		stk_object.pop();
			temp_top_predicate = stk_predicate.top();
			stk_predicate.pop();
			while (stk_predicate.top() == temp_top_predicate)
			{
				ofs <<  stk_object.top() << ",";
	 			stk_object.pop();
				temp_top_predicate = stk_predicate.top();
				stk_predicate.pop();
			}
			//stk_predicate.push(temp_top_predicate);
			ofs <<  stk_object.top() << "],\n";
	 		stk_object.pop();


		}
	}
	
	
	ofs << "\t" << stk_predicate.top() << ":" << stk_object.top() << ",\n";
	stk_object.pop();
	stk_predicate.pop();
	ofs << "},\n";


}//add_json_doc



//******************************************************
//
//******************************************************
std::string get_subject(string& triple ){



	unsigned first, last;
	string subject;
	static const size_t npos = -1;
	
		first = 0;
		last = triple.find(">");
		subject = triple.substr (0,last);
		triple = triple.substr (last+1,triple.length());
		while ( subject.find("/") != string::npos )
		{
		
			first = subject.find("/");

			//cout << "*****First: " << first << "  Length:  " << subject.length() << " Subject:  " << subject << endl;
			
			subject = subject.substr (first+1, subject.length());

			//cout << "After ####### Length:  " << subject.length() << " Subject:  " << subject << endl;
			
			
			
			
		}

/*
		subject.erase(std::remove(subject.begin(), subject.end(), '/'), subject.end());		
		subject.erase(std::remove(subject.begin(), subject.end(), '\"'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '\''), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '\\'), subject.end());	
		subject.erase(std::remove(subject.begin(), subject.end(), ';'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '.'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '$'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '*'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '<'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '>'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), ':'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '|'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '?'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '_'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '%'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '-'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '!'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '~'), subject.end());		
		subject.erase(std::remove(subject.begin(), subject.end(), '#'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '^'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '&'), subject.end());		
		subject.erase(std::remove(subject.begin(), subject.end(), '('), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), ')'), subject.end());
		subject.erase(std::remove(subject.begin(), subject.end(), '+'), subject.end());		
		subject.erase(std::remove(subject.begin(), subject.end(), '='), subject.end());		
		subject.erase(std::remove(subject.begin(), subject.end(), '['), subject.end());		
		subject.erase(std::remove(subject.begin(), subject.end(), ']'), subject.end());		
		subject.erase(std::remove(subject.begin(), subject.end(), '{'), subject.end());		
		subject.erase(std::remove(subject.begin(), subject.end(), '}'), subject.end());		
		subject.erase(std::remove(subject.begin(), subject.end(), ','), subject.end());		
		subject.erase(std::remove(subject.begin(), subject.end(), '\t'), subject.end());	
		subject.erase(std::remove(subject.begin(), subject.end(), ' '), subject.end());		
*/		
		
						
		
		subject = "\"" + subject + "\"";

return subject;
}



//******************************************************
//
//******************************************************
std::string get_predicate(string& triple ){



	unsigned first, last;
	string predicate;
		static const size_t npos = -1;

		first = 0;
		last = triple.find(">");
		predicate = triple.substr (0,last);
		triple = triple.substr (last+1,triple.length());
		while (predicate.find("/") != string::npos)
		{
			first = predicate.find("/");
			predicate = predicate.substr (first+1, predicate.length());
			//cout << subject << endl;
		}


		if ( predicate.find("#") < predicate.length() )
			predicate = predicate.substr ((predicate.find("#"))+1, predicate.length());

		predicate = "\"" + predicate + "\"";
		//cout << predicate << endl;

	
return predicate;
}



//******************************************************
//
//******************************************************



bool is_number(string str){

	for(size_t i=0; i < str.length() ; i++)
	{
		if (!isdigit( str[ i ] ))
			return false;
	}

return true;
}//is_number










//******************************************************
//
//******************************************************
std::string get_object(string& triple){


	unsigned first, last;
	string object;
	char flag = 't';


		first = 0;
		

		if (homepage_indicator == 'Y')
		{
			first = triple.find("<");
			last = triple.find(">");
			object = triple.substr (first+1, last-3);
			//object = "\"" + object + "\"";
			homepage_indicator = 'N';


		}else if ( (triple.find(">") < triple.length())  && (triple.find("^") > triple.length()) )
		{
			object = triple.substr (0,triple.find(">"));
			if (object.substr(object.length()-2,object.length()).find("/") <= object.length())
				object = triple.substr (0,triple.find(">")-1);


			while (first < object.length())
			{
				first = object.find("/");
				object = object.substr (first+1, object.length());
				//cout << subject << endl;
			}
				if ( object.find("#") <= object.length())
				{
					first = object.find("#");
					object = object.substr (first+1, object.length());
				}

				//object = "\"" + object + "\"";
		}
		else if (triple.find(">") < triple.length()  && (triple.find("^") < triple.length()) )
		{
			//flag = 'f';
			
			first = triple.find("\"");			
			last  = triple.substr (first+1,triple.length()).find("\"");



			if (is_number(triple.substr (first+1,last)) || ( triple.substr (first+1,last).find(".") <= triple.substr (first+1,last).length() &&
			triple.substr (first+1,last).find(":") > triple.substr (first+1,last).length() )) 
			{
				object = triple.substr (first+1,last);
				//cout << "number-->" << object << "\n";
				flag = 'f';

			}else if ( triple.substr (first+1,last).find("-") <= triple.substr (first+1,last).length() )  
			{
				object = triple.substr (first+1,last);

		

					if (object.find("T") <= object.length())
						object = object.substr (0,object.find("T"));// + "\"";

					//object = "{\"$date\":\"" + object + "T00:00:00.000Z\"}";  //ISODate("2013-10-01T00:00:00.000Z"),
					//object = "ISODate(\"" + object + "T00:00:00.000Z\")";  //ISODate("2013-10-01T00:00:00.000Z"),
					//object = "\"" + object + "T00:00:00.000Z\"";  //ISODate("2013-10-01T00:00:00.000Z"),
					//object = "\"" + object + "\"";
			
				
			
			}else
			{
				
				object = triple.substr (first,last+2);			
				
			
			}

		
			

			
			


			

		}else
		{
			first = triple.find("\"");
			object = triple.substr (first,triple.length()-7) +
				 triple.substr (triple.length()-5,triple.length())+
				 "\""; 
			
			

		}
		



	
	if (flag != 'f')
	{
		object.erase(std::remove(object.begin(), object.end(), '\n'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\a'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\b'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\t'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\v'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\f'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\r'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\x1B'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\0'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\00'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\000'), object.end());
		
		object.erase(std::remove(object.begin(), object.end(), '\"'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '('), object.end());
		object.erase(std::remove(object.begin(), object.end(), ')'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '?'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\''), object.end());
		object.erase(std::remove(object.begin(), object.end(), '\\'), object.end());
		
		//boost::replace_all(name, "\"", "\\\"");			
		
		object.erase(std::remove(object.begin(), object.end(), '*'), object.end());		
		object.erase(std::remove(object.begin(), object.end(), '#'), object.end());		
		object.erase(std::remove(object.begin(), object.end(), ';'), object.end());
		object.erase(std::remove(object.begin(), object.end(), '%'), object.end());
		
		object = "\"" + object + "\"";
	
	}


return object;
}


