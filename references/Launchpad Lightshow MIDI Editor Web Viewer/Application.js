var baseURL=new CFURL(".",ObjectiveJ.pageURL);
var URLCache={};
var CFHTTPRequest_open=CFHTTPRequest.prototype.open;
CFHTTPRequest.prototype.open=function(_1,_2,_3,_4,_5){
var _6=CFHTTPRequest._lookupCachedRequest(_2);
if(_6){
var _7=this;
this._nativeRequest=_6;
this._nativeRequest.onreadystatechange=function(){
ObjectiveJ.determineAndDispatchHTTPRequestEvents(_7);
};
}
return CFHTTPRequest_open.apply(this,arguments);
};
CFHTTPRequest._cacheRequest=function(_8,_9,_a,_b){
URLCache[_8]=new MockXMLHttpRequest(_9,_a,_b);
};
CFHTTPRequest._lookupCachedRequest=function(_c){
return URLCache[_c];
};
function MockXMLHttpRequest(_d,_e,_f){
this.readyState=CFHTTPRequest.UninitializedState;
this.status=_d||200;
this.statusText="";
this.responseText=_f||"";
this._responseHeaders=_e||{};
};
MockXMLHttpRequest.prototype.open=function(_10,url,_11,_12,_13){
this.readyState=CFHTTPRequest.LoadingState;
this.async=_11;
};
MockXMLHttpRequest.prototype.send=function(_14){
var _15=this;
_15.responseText=_15.responseText.toString();
function _16(){
for(_15.readyState=CFHTTPRequest.LoadedState;_15.readyState<=CFHTTPRequest.CompleteState;_15.readyState++){
_15.onreadystatechange();
}
};
(_15.async?ObjectiveJ.Asynchronous(_16):_16)();
};
MockXMLHttpRequest.prototype.onreadystatechange=function(){
};
MockXMLHttpRequest.prototype.abort=function(){
};
MockXMLHttpRequest.prototype.setRequestHeader=function(_17,_18){
};
MockXMLHttpRequest.prototype.getAllResponseHeaders=function(){
return this._responseHeaders;
};
MockXMLHttpRequest.prototype.getResponseHeader=function(_19){
return this._responseHeaders[_19];
};
MockXMLHttpRequest.prototype.overrideMimeType=function(_1a){
};
var appFilesCount=2;
for(var i=1;i<=appFilesCount;i++){
var script=document.createElement("script");
script.src="Application"+i+".js";
script.charset="UTF-8";
script.onload=function(){
if(--appFilesCount===0){
ObjectiveJ.bootstrap();
}
};
document.getElementsByTagName("head")[0].appendChild(script);
}
