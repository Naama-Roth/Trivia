#include "LoginRequestHandler.h"

LoginRequestHandler::LoginRequestHandler(LoginManager loginManager, RequestHandlerFactory* handlerFactory) : m_loginManager(loginManager), m_handleFactory(*handlerFactory) {};


bool LoginRequestHandler::isRequestRelevant(RequestInfo& r)
{
	return (char)r.requestId == LOGIN_ || (char)r.requestId == SIGNUP_;
}

RequestResult LoginRequestHandler::handleRequest(RequestInfo r)
{
	string usName;
	IResponse* response = nullptr;

	if (isRequestRelevant(r))
	{
		if ((char)r.requestId == SIGNUP)
		{
			SignUpRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignUpRequests(r.buffer.c_str());
			
			//need to check success or fail
			response = new SignupResponse(signup(signupRequest.username, signupRequest.password, signupRequest.email)); //create status and signup the user (or doesnt signup if cant)
			std::cout << "res log req handle: " << response->responseType << '\n';
			usName = signupRequest.username;
		}
		else if ((char)r.requestId == LOGIN)
		{
			LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequests(r.buffer.c_str());

			//need to check success or fail
			response = new LoginResponse(login(loginRequest.username, loginRequest.password));
			usName = loginRequest.username;
		}
	}
	else
	{
		response = &ErrorResponse("Request isn't relevant");
	}


	RequestResult result;
	string rRes = JsonResponsePacketSerializer::SerializeResponse(response);
	result.response = rRes;
	result.newHandler = m_handleFactory.createMenuRequestHandler(m_loginManager.getLoggedUser(usName));
	return result;
}


int LoginRequestHandler::login(string username, string password)
{
	return m_loginManager.login(username, password) ? SUCCESS : FAIL;
}

int LoginRequestHandler::signup(string username, string password, string email)
{
	return m_loginManager.signup(username, password, email) ? SUCCESS : FAIL;
}