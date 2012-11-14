/*
 * parser.h
 *
 *  Created on: Oct 15, 2012
 *      Author: chjd
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <fstream>
using std::fstream;

#include <iostream>
//using std::cerr;
using std::cout;
using std::endl;


//using std::ofstream;
//extern ofstream errorlog;

using std::clog;

#include "stringenc.h"
#include "macro.h"
#include "circuit.h"

class Error{};

class Parser {
public:
	Parser() {
	}
	Parser(Circuit* ckt):ckt(ckt){

	}
	bool Run(string fname){
		if(!Init(fname))
			return false;
		if(!Parse())
			return false;
		if(!Finish())
			return false;
		return true;
	}
private:
	bool Init(string fname) {
		fp.open(fname);
		line = "";
		linecnt = 0;
		FEOF = false;
		if(!fp.is_open())
			clog << "failed to open file:" << fname << endl;
		return fp.is_open();
	}

	bool Finish() {
		fp.close();
		if(FEOF)
			clog << "Parse succeed." << endl;
		else
			clog << "Parse failed." << endl;
		return FEOF;
	}

	bool Parse() {
		bool ok = true;
		try {
			while (!FEOF) {
				GetLine();
				MatchLineGrammar();
			}
		} catch (Error& err) {
			ok = false;
		} catch (...) {
			ok = false;
		}
		return ok;
	}

	void GetLine() {
		do {
			getline(fp, line);
			linecnt += 1;
			line = rtrim(line);
			//clog << linecnt << "\t" << line << "\t" << line.size() << endl;
		} while (line.empty() && !fp.eof());
		if (line.empty()) {
			errorInfo("\n\t unexpected end-of-file\n");
			throw Error();
		}
		line = stringtolower(line);
//		clog << linecnt << "\t" << line << "\t" << line.size() << endl;
		linecol = 0;
		c = line[linecol];
	}

	void MatchLineGrammar() {
		switch (c) {
		case ' ':
			EmptyLine();
			break;
		case '*':
			CommentLine();
			break;
		case '.':
			CmdLine();
			break;
		default:
			ElemLine();
			break;
		}
	}

private:
	/*---------------------------------------------------------------*/
	/*------------------- lexer -------------------------------------*/

	/* lexer match char */
	bool isCHAR(char t){
		return c==t;
	}
	bool isWS() {
		return (c == ' ' || c == '\t'); // '\n' will be rtrimed
	}
	bool isTEXT() {
		return (c >= 'a' && c <= 'z');
	}
	bool isDIGIT() {
		return (c >= '0' && c <= '9');
	}
	bool isTEXT_NUM() {
		return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c == '_');
	}
	bool isUNIT() {
		return (c == 'f' || c == 'p' || c == 'n' || c == 'u' || c == 'm' || c
				== 'k' || c == 'x' || c == 'g');
	}
	/* lexer match char end */

	void consume() {
		linecol++;
		if (linecol >= line.length())
			c = _eof;
		else
			c = line[linecol];
		//clog << "consume:" << c << endl;
	}

	// basic match, consume whitespace
	void matchWS() {
		while (isWS()) {
			consume();
		}
	}

	// match
	/* lexer next token */

	// integer, \d+
	string matchDigit() {
		string res;
		while (isDIGIT()) {
			res.push_back(c);
			consume();
		}
		return res;
	}

	string matchSign(){
		string res;
		if(c=='+' || c=='-'){
			res.push_back(c);
			consume();
		}
		return res;
	}

	void matchDigit(int& value){
		string buf = matchSign();
		buf += matchDigit();
		value = str2int(buf);
	}

	// integer or float, [-+]?(\d+) ([.](\d*))? ((e)[-+]?(\d+))?
	bool matchFloat(double& value) {
		string token;
		token = matchSign();
		token += matchDigit();
		if (c == '.') {
			token.push_back(c);
			consume();
			token += matchDigit();
		}
		if (c == 'e') {
			token.push_back(c);
			consume();
			token += matchSign();
			token += matchDigit();
		}
		value = str2float(token);
		return !(token.empty());
	}

	// text, [a-z]+,
	// if empty
	string matchText() {
		string token;
		while (isTEXT()) {
			token.push_back(c);
			consume();
		}
		return token;
	}

	// match all text, till whitespace
	string matchAText(){
		string buf;
		while(!isWS() && (c!=_eof)){
			buf.push_back(c);
			consume();
		}
		return buf;
	}

	// id or node, can be letter or digit or '_'
	string matchText_Num() {
		string token;
		while (isTEXT_NUM()) {
			token.push_back(c);
			consume();
		}
		return token;
	}

	// unit, char(or string) [f p n u m k x g ]
	char matchUnit() {
		char token = ' ';
		if (isUNIT()){
			token = c;
			consume();
		}
		return token;
	}

	/* lexer next token end*/

	/* lexer try to match */

	// back up and resume
	int backP(){
		return linecol;
	}
	void resumeP(int backp){
		linecol = backp;
		if(linecol>=line.length())
			c = _eof;
		else
			c = line[linecol];
	}

	// try to match param value, float + unit?, return double value directly
	bool try_matchParamValue(double& param) {
		int backp = backP();
		//string token = matchFloat();
		bool ok = matchFloat(param);
		//if (!token.empty()) {
		if (ok) {
			//param = str2float(token);
			param *= UnitValue(matchUnit());
			return true;
		} else {
			resumeP(backp);
			return false;
		}
	}

	// try to match param value , will get both param value & ExType
	bool try_matchParamValue(double& param, ExType& tp) {
		int backp = backP();
		//string token = matchFloat();
		matchFloat(param);
		//if (!token.empty()) {
		if(param){
			//param = str2float(token);
			tp = ExType::NONE;
			param *= UnitValue(matchUnit());
			return true;
		} else {
			resumeP(backp);
			return false;
		}
	}

	// try to match extreme value, inf or zero,
	/* when this match, both param value and ExType are decided
	 * the sign of inf was not considered
	 * */
	bool try_matchParamExValue(double& param, ExType& tp) {
		int backp = backP();
		string buf = matchText();
		if (buf == "inf") {
			tp = ExType::INF;
			param = ExValue(tp);
			return true;
		} else if (buf == "zero") {
			tp = ExType::ZERO;
			param = ExValue(tp);
			return true;
		} else {
			resumeP(backp);
			tp = ExType::NONE;
			return false;
		}
	}

	// try to match fault, short or open, or just fault
	bool try_matchFault(FaultType& tp) {
		int backp = backP();
		string buf = matchText();
		if (buf == "fault")
			tp = FaultType::FAULT;
		else if (buf == "short")
			tp = FaultType::SHORT;
		else if (buf == "open")
			tp = FaultType::OPEN;
		else {
			resumeP(backp);
			tp = FaultType::NONE;
			return false;
		}
		return true;
	}

	bool try_matchMark(string mark){
		matchWS();
		string buf = matchText();
		return buf==mark;
	}

	bool try_matchKeyParam(unordered_map<string,double>& keyParam){
		int backp = backP();
		bool ok = false;
		while(true){
			matchWS();
			string buf = matchText();
			if(buf.empty())
				break;
			auto it = keyParam.find(buf);
			if(it!=keyParam.end()){
				matchWS();
				if(isCHAR('=')){
					consume();
					matchWS();
				}
				double param;
				if(!try_matchParamValue(param)){
					errorInfo(c);
					TipTokenKeyParam(buf);
					throw Error();
				}
				keyParam[buf] = param;
				ok = true;
			}
			else
				break;
		}
		if(!ok){
			resumeP(backp);
		}
		return ok;
	}

	/* lexer try to match end */

	/* lexer must match */
	// to match delimiter, between each token, whitespace as delimiter
	void matchDelim() {
		if (!isWS() && c != _eof && c != '\n') {
			errorInfo(c);
			throw Error();
		}
	}

	// match end of line
	bool matchLEOF() {
		matchWS();
		//clog << line << endl;
		//clog << "leof:" << int(c) << "\t" << int(_eof) << "\t" << linecol << endl;
		return (c==_eof);
	}

	void matchIdNode(string& token) {
		matchWS();
		token = matchText_Num();
		matchDelim();
	}

	// to match param, maybe param value or extreme value, so use try option
	/* or use predicate,
	 * 	if isDIGIT or isDigitSign, then to match param value,
	 * 	else to match inf/zero
	 * */
	void matchParam(double& param, ExType& exTp) {
		matchWS();
		if (try_matchParamValue(param))
			matchDelim();
		else if (try_matchParamExValue(param, exTp))
			matchDelim();
		else {
			errorInfo(c);
			throw Error();
		}
	}
	/* lexer must match end */

	void matchPhase(int& phase){
		matchWS();
		int backp = backP();
		string buf = matchDigit();
		phase = 0;
		if(!buf.empty()){
			phase = str2int(buf);
			return;
		}
		buf = matchText();
		if(buf=="phase"){
			matchWS();
			if(isCHAR('=')){
				consume();
				matchWS();
			}
			backp = backP();
			string buf = matchDigit();
			phase = str2int(buf);
		}
		if(phase==1 || phase==2)
			return;
		else{
			errorInfo(c);
			TipTokenPhase();
			throw Error();
		}
	}

	/*------------------- lexer -------------------------------------*/
	/*---------------------------------------------------------------*/

	bool matchMark(string target) {
		matchWS();
		bool ok = true;
		for(auto i=0;i<target.length();i++){
			if(c==target[i])
				consume();
			else{
				ok = false;
				break;
			}
		}
		if(!ok){
			errorInfo(c);
			throw Error();
		}
		return ok;
		/*
		string token = matchText();
		if (token == strtolower(target))
			return true;
		else {
			errorInfo(token);
			throw Error();
			return false;
		}*/
	}

	bool matchMarks(string& token, vector<string> targets) {
		matchWS();
		token = matchText();
		//foreach(targets,it)
		for (auto it = targets.begin(), et = targets.end(); it != et; it++) {
			if (strtolower((*it)) == token)
				return true;
		}
		errorInfo(token);
		return false;
	}

	/*---------------------------------------------------------------*/
	/*------------------- parser ------------------------------------*/

	// if first letter of line is whitespace
	void EmptyLine() {
		line = ltrim(line);
		if (line.empty() || line[0] == '*'){
			//GetLine();
		}
		else {
			clog << "Warning at line " << linecnt << ":\n\t";
			clog << "this line will be ignored." << endl;
		}
	}

	// if first letter is '*', comment line, skip
	void CommentLine() {
		/* skip */
		//GetLine();
		return;
	}

	// if first letter is alpha a-z, may be valid elem line.
	void ElemLine() {
		switch (line[0]) {
		case 'v':
			matchElemVsrc();
			break;
		case 'i':
			matchElemIsrc();
			break;
		case 'r':
			matchElemRes();
			break;
		case 'c':
			matchElemCap();
			break;
		case 'l':
			matchElemInd();
			break;
		case 'e':
			matchElemVCVS();
			break;
		case 'g':
			matchElemVCCS();
			break;
		case 's':
			matchElemSwitch();
			break;
		default: {
			errorInfo(c);
			throw Error();
		}
		}
	}

	void CmdLine() {
		//clog << "to match cmd line" << endl;
		string buf = matchAText();
		//clog << linecol << ":" << buf << endl;
		if(buf==".option")
			matchDotOption();
		else if(buf==".ac")
			matchDotAC();
		else if(buf==".print")
			matchDotPrint();
		else if(buf==".clock")
			matchDotClock();
		else if(buf==".sc")
			matchDotScInOut();
		else if(buf==".end")
			matchDotEnd();
		else{
			errorInfo(buf);
			throw Error();
		}
	}

	// to match resistor line
	void matchElemRes(){
		string name, n1, n2;
		double param;
		ExType inf;
		FaultType fault;

		matchIdNode(name);
		matchIdNode(n1);
		matchIdNode(n2);
		matchParam(param,inf);
		try_matchFault(fault);
		//cout << "to match res leof: "<< linecol << "\t" << c << "c"<< endl;
		if (!matchLEOF()){
			errorInfo(c);
			TipGrammar("resistor",'R');
			throw Error();
		}

		int nodep,noden;
		nodep = ckt->AddNode(n1);
		noden = ckt->AddNode(n2);
		ElemRes* elem = new ElemRes(name,nodep,noden,param);
		elem->InfZero(inf);
		elem->Fault(fault);
		bool ok = ckt->AddElem(elem);
		if(!ok){
			errorInfo("wrong repeated element name");
			throw Error();
		}
	}

	// to match capacitor line
	void matchElemCap(){
		string name, n1, n2;
		double param;
		ExType inf;
		FaultType fault;

		matchIdNode(name);
		matchIdNode(n1);
		matchIdNode(n2);
		matchParam(param,inf);
		try_matchFault(fault);
		if (!matchLEOF()){
			errorInfo(c);
			TipGrammar("capacitor",'C');
			throw Error();
		}

		int nodep,noden;
		nodep = ckt->AddNode(n1);
		noden = ckt->AddNode(n2);
		ElemCap* elem = new ElemCap(name,nodep,noden,param);
		elem->InfZero(inf);
		elem->Fault(fault);
		bool ok = ckt->AddElem(elem);
		if(!ok){
			errorInfo("wrong repeated element name");
			throw Error();
		}
	}

	// to match inductor line
	void matchElemInd() {
		string name, n1, n2;
		double param;
		ExType inf;
		FaultType fault;

		matchIdNode(name);
		matchIdNode(n1);
		matchIdNode(n2);
		matchParam(param, inf);
		try_matchFault(fault);
		if (!matchLEOF()) {
			errorInfo(c);
			TipGrammar("capacitor", 'L');
			throw Error();
		}

		int nodep, noden;
		nodep = ckt->AddNode(n1);
		noden = ckt->AddNode(n2);
		ElemInd* elem = new ElemInd(name, nodep, noden, param);
		elem->InfZero(inf);
		elem->Fault(fault);
		bool ok = ckt->AddElem(elem);
		if (!ok) {
			errorInfo("wrong repeated element name");
			throw Error();
		}
	}

	// to match switch line
	void matchElemSwitch() {
		string name, n1, n2;
		int phase;
		FaultType fault;

		matchIdNode(name);
		matchIdNode(n1);
		matchIdNode(n2);
		matchPhase(phase);
		matchWS();
		try_matchFault(fault);
		if (!matchLEOF()) {
			errorInfo(c);
			TipGrammarSw();
			throw Error();
		}

		int nodep, noden;
		nodep = ckt->AddNode(n1);
		noden = ckt->AddNode(n2);
		ElemSwitch* elem = new ElemSwitch(name, nodep, noden, phase);
		elem->Fault(fault);
		bool ok = ckt->AddElem(elem);
		if (!ok) {
			errorInfo("wrong repeated element name");
			throw Error();
		}
	}

	// to match voltage source
	void matchElemVsrc() {
		string name, n1, n2;
		double param;
		unordered_map<string, double> keyParamMap = { { "dc", 0.0 },
				{ "ac",0.0 } };

		matchIdNode(name);
		matchIdNode(n1);
		matchIdNode(n2);
		matchWS();
		if (try_matchParamValue(param)) {
			keyParamMap["ac"] = param;
		}
		else {
			matchWS();
			bool ok = try_matchKeyParam(keyParamMap);
			if (!ok || (!matchLEOF())) {
				errorInfo(c);
				TipGrammarSrc("volatge source", 'V');
				throw Error();
			}
		}

		int nodep, noden;
		nodep = ckt->AddNode(n1);
		noden = ckt->AddNode(n2);
		ElemVsrc* elem = new ElemVsrc(name, nodep, noden, keyParamMap["dc"],
				keyParamMap["ac"]);
		bool ok = ckt->AddElem(elem);
		if (!ok) {
			errorInfo("wrong repeated element name");
			throw Error();
		}
	}

	// to match current source
	void matchElemIsrc() {
		string name, n1, n2;
		double param;
		unordered_map<string, double> keyParamMap = { { "dc", 0.0 },
				{ "ac",0.0 } };

		matchIdNode(name);
		matchIdNode(n1);
		matchIdNode(n2);
		matchWS();
		if (try_matchParamValue(param)) {
			keyParamMap["ac"] = param;
		} else {
			matchWS();
			bool ok = try_matchKeyParam(keyParamMap);
			if (!ok || (!matchLEOF())) {
				errorInfo(c);
				TipGrammarSrc("current source", 'I');
				throw Error();
			}
		}

		int nodep, noden;
		nodep = ckt->AddNode(n1);
		noden = ckt->AddNode(n2);
		ElemIsrc* elem = new ElemIsrc(name, nodep, noden, keyParamMap["dc"],
				keyParamMap["ac"]);
		bool ok = ckt->AddElem(elem);
		if (!ok) {
			errorInfo("wrong repeated element name");
			throw Error();
		}
	}

	// to match vcvs or opamp
	void matchElemVCVS(){
		string name, n1, n2, n3, n4;
		double param;
		ExType inf;

		bool opamp = false;

		matchIdNode(name);
		matchIdNode(n1);
		matchIdNode(n2);
		opamp = try_matchMark("opamp");

		matchIdNode(n3);
		matchIdNode(n4);
		if(!opamp)
			matchParam(param, inf);

		if (!matchLEOF()){
			errorInfo(c);
			if(opamp)
				TipGrammarOpamp();
			else
				TipGrammarTwoPort("VCVS",'E');
			throw Error();
		}

		int nodep,noden, nodepp, nodenn;
		nodep = ckt->AddNode(n1);
		noden = ckt->AddNode(n2);
		nodepp = ckt->AddNode(n3);
		nodenn = ckt->AddNode(n4);

		bool ok;
		if(opamp || inf==ExType::INF){
			ElemOpamp* elem = new ElemOpamp(name,nodep,noden,nodepp,nodenn);
			ok = ckt->AddElem(elem);
		}
		else{
			ElemVCVS* elem = new ElemVCVS(name,nodep,noden,nodepp,nodenn,param);
			ok = ckt->AddElem(elem);
		}
		if(!ok){
			errorInfo("wrong repeated element name");
			throw Error();
		}
	}

	// to match vccs
	void matchElemVCCS(){
		string name, n1, n2, n3, n4;
		double param;
		ExType inf;

		matchIdNode(name);
		matchIdNode(n1);
		matchIdNode(n2);
		matchIdNode(n3);
		matchIdNode(n4);
		matchParam(param,inf);

		//try_matchFault(fault);
		if (!matchLEOF()){
			errorInfo(c);
			TipGrammarTwoPort("VCCS",'G');
			throw Error();
		}

		int nodep,noden, nodepp, nodenn;
		nodep = ckt->AddNode(n1);
		noden = ckt->AddNode(n2);
		nodepp = ckt->AddNode(n3);
		nodenn = ckt->AddNode(n4);
		ElemVCCS* elem = new ElemVCCS(name,nodep,noden,nodepp,nodenn, param);
		elem->InfZero(inf);
		bool ok = ckt->AddElem(elem);
		if(!ok){
			errorInfo("wrong repeated element name");
			throw Error();
		}
	}

	// to match cmd line
	void matchDotOption(){
		//matchMark(".option");
		// other words donot care
		return;
	}

	void matchSpType(SpType& sp){
		string buf = matchText();
		if(buf=="lin")
			sp = SpType::LIN;
		else if(buf=="dec")
			sp = SpType::DEC;
		else{
			errorInfo(buf);
			throw Error();
		}
	}

	// to match AC task
	void matchDotAC(){
		SpType sp;
		int num;
		double sf,ef;

		//matchMark(".ac");
		matchWS();
		matchSpType(sp);
		matchWS();
		num = str2int(matchDigit());
		matchWS();
		try_matchParamValue(sf);
		matchWS();
		try_matchParamValue(ef);

		clog << "num: " << num << endl;
		clog << "sf: " << sf << endl;
		clog << "ef: " << ef << endl;

		if (!matchLEOF()){
			errorInfo(c);
			TipGrammarDotAC();
			throw Error();
		}
		Task* task = new TaskAC(sp,num,sf,ef);
		bool ok = ckt->AddTask(task);
		if(!ok){
			errorInfo("some thing wrong");
			throw Error();
		}
	}

	void matchDotPrint(){
		//matchMark(".print");
		matchWS();
		string buf = matchText();
		Task* task = nullptr;
		if(buf=="ac")
			task = ckt->GetLastTaskByType(TaskType::AC);
		if(task==nullptr){
			errorInfo(buf);
			TipGrammarDotPrint();
			throw Error();
		}
		matchWS();
		string v = matchText();
		PrintType vtp;
		if(v=="vdbp")
			vtp = PrintType::DBP;
		else if(v=="vmp")
			vtp = PrintType::MP;
		else if(v=="vri")
			vtp = PrintType::RI;
		else{
			errorInfo(v);
			throw Error();
		}
		matchWS();
		matchMark("(");
		string n = matchDigit();
		matchWS();
		matchMark(")");
		if (!matchLEOF()){
			errorInfo(c);
			TipGrammarDotPrint();
			throw Error();
		}
		int nn = ckt->AddNode(n);
		((TaskAC*)task)->AddPrintTask(vtp,nn);

/*		bool ok = ckt->AddTask(task);
		if(!ok){
			errorInfo("some thing wrong");
			throw Error();
		}
*/
	}

	void matchDotClock()
	{
		matchWS();
		double clk;
		try_matchParamValue(clk);
		if (!matchLEOF())
		{
			errorInfo(c);
			TipGrammarDotEnd();
			throw Error();
		}
		ckt->SetClk(clk);
	}

	void matchDotScInOut()
	{
		matchWS();
		string buf = matchText();
		int inout = 0;
		if(buf=="voo")
			inout = 1;
		else if(buf=="voe")
			inout = 2;
		else if(buf=="veo")
			inout = 3;
		else if(buf=="vee")
			inout = 4;
		else{
			errorInfo(buf);
			throw Error();
		}
		ckt->SetScInOut(inout);
		ckt->SetScFlag(true);
	}

	// to match .end, FEOF = true
	void matchDotEnd(){
		//matchMark(".end");
		if (!matchLEOF()){
			errorInfo(c);
			TipGrammarDotEnd();
			throw Error();
		}
		FEOF = true;
	}
	/*------------------- parser ------------------------------------*/
	/*---------------------------------------------------------------*/



	/*---------------------------------------------------------------*/
	/*------------------- error info --------------------------------*/
	/* show error info, member function of parser, easy to get linecnt and linecol */

	/* error info format:
	 * Error at line.col:
	 * 		error info.
	 * */

	/* exception & error
	 * when error happens, show error info,
	 * then trace back by exception mechanism
	 * */

	// unexpected char
	void errorInfo(char t) {
		clog << "Error at line " << linecnt << "." << linecol << ":\n\t";
		clog << "unexpected character '" << toString(t) << "'." << endl;
	}

	// unexpected token
	void errorInfo(string token) {
		clog << "Error at line " << linecnt << "."
				<< (linecol - token.length()) << ":\n\t";
		clog << "unexpected token \" " << token << "\"." << endl;
	}

	// specified error info
	void errorInfo(const char* info) {
		clog << "Error at line " << linecnt << "." << linecol << ":\n\t";
		clog << info << "." << endl;
	}

	/* error info end */
	/*------------------- error info --------------------------------*/
	/*---------------------------------------------------------------*/

	/*---------------------------------------------------------------*/
	/*------------------- syntax tips -------------------------------*/

	void TipTokenIdNum(){
		clog << "!\tvalid token for id or node only contains character 'a'-'z' or '0'-'9' or '_' " << endl;
	}
	void TipTokenParam(){
		clog << "!\tvalid token for param can be float number or inf/zero" << endl;
	}
	void TipTokenKeyParam(){
		clog << "!\tvalid key param format is param_name [=] param_value" << endl;
	}
	void TipTokenKeyParam(string buf){
		clog << "!\tvalid key param format is " << buf << " [=] param_value" << endl;
	}
	void TipTokenPhase(){
		clog << "!\tvalid phase format is [phase [=]] 1/2" << endl;
	}
	void TipGrammar(string name,char type){
		clog << "! Grammar for " << name <<":" << endl;
		clog << "\t" << type << "name" << " n+ n- value [fault]" << endl;
	}
	void TipGrammarSrc(string name,char type){
		clog << "! Grammar for " << name <<":" << endl;
		clog << "\t" <<  type << "name" << " n+ n- [DC value] [AC value]" << endl;
	}
	void TipGrammarTwoPort(string name,char type){
		clog << "! Grammar for " << name <<":" << endl;
		clog << "\t" <<  type << "name" << " n+ n- nc+ nc- value [fault]" << endl;
	}
	void TipGrammarOpamp(){
		clog << "! Grammar for OPAMP:" << endl;
		clog << "\t Ename n+ n- OPAMP nc+ nc- value [fault]" << endl;
	}
	void TipGrammarSw(){
		clog << "! Grammar for switch:" << endl;
		clog << "\t Sname n+ n- phase [fault]" << endl;
	}

	void TipGrammarDotAC(){
		clog << "! Grammar for ac command:" << endl;
		clog << "\t .ac \"lin\"|\"dec\" num_of_points start_frequency end_frequency" << endl;
	}
	void TipGrammarDotPrint(){
		clog << "! Grammar for print command:" << endl;
		clog << "\t .print ac V[DB|P|R|I](node[,node])" << endl;
	}
	void TipGrammarDotEnd(){
		clog << "! Grammar for end command:" << endl;
		clog << "\t .end" << endl;
		clog << "\t this is the end-of-file tag" << endl;
	}
	/*------------------- syntax tips -------------------------------*/
	/*---------------------------------------------------------------*/

private:
	// circuit
	Circuit* ckt;
	// parser
	fstream fp;
	int linecnt;
	bool FEOF;
	// lexer
	string line;
	char c;
	int linecol;
	const static char _eof = char(-1);
};

#endif /* PARSER_H_ */
