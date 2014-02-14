ko.bindingHandlers.min =
{
	update: function(element, valueAccessor, allBindings, viewModel, bindingContext) {
		var min = ko.unwrap(valueAccessor());
		var max = allBindings.get('max');
		var value = $(element).val();
		
		if(max != undefined && max < min)
		{
			min = max;
		}
		
		if(value < min)
		{
			// We need to call change() after updating the value in order for knockoutjs to
			// update its observables
			$(element).val(min).change();
		}
		else
		{
			// We need to call change() in order for knockoutjs to
			// update its observables
			$(element).change();
		}

		element.min = min;
	}
};

ko.bindingHandlers.max =
{
	update: function(element, valueAccessor, allBindings, viewModel, bindingContext) {
		var max = ko.unwrap(valueAccessor());
		var min = allBindings.get('min');
		var value = $(element).val();
		
		if(min != undefined && max < min)
		{
			max = min;
		}
		
		if(value > max)
		{
			// We need to call change() after updating the value in order for knockoutjs to
			// update its observables
			$(element).val(max).change();
		}
		else
		{
			// We need to call change() in order for knockoutjs to
			// update its observables
			$(element).change();
		}

		element.max = max;
	}
};

ko.bindingHandlers.commandConsoleKeyActions = 
{
	init: function(element, valueAccessor, allBindingsAccessor, viewModel)
	{
		$(element).keyup(function(e)
		{
			if(e.which === 13)
			{
				// Return key
				viewModel.sendCommand();
			}
			else if(e.which === 38)
			{
				// Up Arrow
				viewModel.previousCommand();
			}
			else if(e.which === 40)
			{
				// Down Arrow
				viewModel.nextCommand();
			}
		});
	}
};

function Joint(data, viewModel)
{
	var self = this;
	self.viewModel = viewModel;
	
	self.title = data.title;
	self.jointNumber = ko.observable(data.jointNumber);
	self.inputId = ko.computed(function()
	{
		return self.title.toLowerCase() + "-angle";
	});
	
	self.min = ko.observable(data.min);
	self.max = ko.observable(data.max);
	self.setPoint = ko.observable(data.setPoint);
	self.currentAngle = ko.observable(self.min() - self.max() / 2);
	self.angleError = ko.observable(0);

	self.PGain = ko.observable(1);
	self.IGain = ko.observable(1);
	self.DGain = ko.observable(1);
	
	// Setpoint received from serial
	self.serialSetPoint = ko.observable((self.setPoint() != 1) ? 1 : 0);
	self.serialSetPoint.subscribe(function()
	{
		self.setPoint(self.serialSetPoint());
	});
	
	// Subscribe to changes in the setpoint so we can send the updates to the 
	// Arduino using the socket
	self.setPoint.subscribe(function()
	{		
		if(self.viewModel.enabled())
		{
			if(self.setPoint() != self.serialSetPoint())
			{
				var command = ko.toJSON(
				{
					command: "setJointAngle",
					jointNumber: self.jointNumber(), 
					angle: self.setPoint()
				});

				console.log(command);

				self.viewModel.socket.send(command);
			}
		}
		else
		{
			console.log("Manual control disabled.")
		}
	});
	
	// Use a computed property to update the Arduino whenever any of the gains change
	ko.computed(function()
	{
		if(self.viewModel.enabled() && self.viewModel.PIDTuningEnabled())
		{
			var command = ko.toJSON(
				{
					command: "setJointGains",
					jointNumber: self.jointNumber(), 
					PGain: parseFloat(self.PGain()).toPrecision(3),
					IGain: parseFloat(self.IGain()).toPrecision(3),
					DGain: parseFloat(self.DGain()).toPrecision(3)
				});

			console.log(command);

			self.viewModel.socket.send(command);
		}
	});
}

function Command(data)
{
	var self = this;
	
	self.command = ko.observable(data.command);
	self.response = ko.observable(data.response);
	self.duration = ko.observable(data.duration || 0);
	self.startTime = Date.now();
	
	self.durationText = ko.computed(function()
	{
		return self.duration() + "ms";
	});
}

function PrometheusViewModel()
{
	// Data
	var self = this;
	self.socket = null;
	//self.socketServer = 'ws://10.33.0.2:8888/';
	self.socketServer = 'ws://localhost:8888/';

	// Enabled Setting
	self.textEnabled = ko.observable("off");
	self.enabled = ko.computed(function()
	{
		return self.textEnabled() == "on" ? true : false;
	});
	
	// Settings Screen Stuff
	self.Render3DEnabledText = ko.observable("on");
	self.Render3DEnabled = ko.computed(function()
	{
		return self.Render3DEnabledText() == "on" ? true : false;
	});
	
	self.Rotate3DEnabledText = ko.observable("off");
	self.Rotate3DEnabled = ko.computed(function()
	{
		return self.Rotate3DEnabledText() == "on" ? true : false;
	});
	
	self.PIDTuningEnabledText = ko.observable("off");
	self.PIDTuningEnabled = ko.computed(function()
	{
		return self.PIDTuningEnabledText() == "on" ? true : false;
	});
	
	self.CommandConsoleEnabledText = ko.observable("off");
	self.CommandConsoleEnabled = ko.computed(function()
	{
		return self.CommandConsoleEnabledText() == "on" ? true : false;
	});
	
	// Joints array
	self.joints = ko.observableArray([]);
	
	// Command Console Data
	self.commandText = ko.observable().extend({ notify: 'always' });
	self.commandHistory = ko.observableArray();
	self.commandPosition = 0;
	
	// 3D Rendering
	self.init3D = function()
	{
		self.renderer = new RobotArm(self);
		
		ko.computed(function()
		{
			self.updateRenderer();
		}).extend({throttle: 100});
		
		self.updateRenderer();
	};
	
	self.updateRenderer = function()
	{
		if(self.Render3DEnabled())
		{
			self.renderer.init();
		}
	};

	// Socket
	self.initSocket = function()
	{
		self.socket = new WebSocket(self.socketServer);

		self.socket.onopen = function()
		{
			console.log('Socket Connected!');
			
			for(var i = 0; i < self.joints().length; i++)
			{
				var command = ko.toJSON(
				{
					command: "getJointGains",
					jointNumber: i
				});
				
				console.log(command);
				self.socket.send(command);
				
				var command = ko.toJSON(
				{
					command: "getJointLimits",
					jointNumber: i
				});
				
				console.log(command);
				self.socket.send(command);
			};
		};

		self.socket.onclose = function()
		{
			console.log('Socket connection lost!');
			self.socket.close();
		};

		self.socket.onmessage = function(event)
		{
			self.receiveSocketData(event.data);
		};
	};

	self.receiveSocketData = function(data)
	{
		// We need to figure out what we are going to do with the command 
		// console now that we are using a socket instead of ajax requests 
		// because we now have to figure out which response belongs to which 
		// command.

		console.log("Data from socket: " + data);

		json = null;

		try
		{
			json = $.parseJSON(data);	
		}
		catch(exception)
		{
			console.log("Failed to parse data from socket as JSON. Data:");
			console.log(data);
		}
		
		if(json != null)
		{
			switch(json.command)
			{
				case "armStatus":
					for(var i = 0; i < self.joints().length; i++)
					{
						self.joints()[i].currentAngle(json.currentAngle[i]);
						self.joints()[i].angleError(json.currentError[i]);
					};
					break;
				
				case "jointAngle":
					self.joints()[json.jointNumber].currentAngle(json.angle);
					break;
				
				case "jointSetPoint":
					self.joints()[json.jointNumber].serialSetPoint(json.setPoint);
					break;
				
				case "jointLimits":
					self.joints()[json.jointNumber].min(json.min);
					self.joints()[json.jointNumber].max(json.max);
					break;
				
				case "jointGains":
					self.joints()[json.jointNumber].PGain(json.PGain);
					self.joints()[json.jointNumber].IGain(json.IGain);
					self.joints()[json.jointNumber].DGain(json.DGain);
					break;

				case "jointError":
					self.joints()[json.jointNumber].angleError(json.error);
					break;
			}
		}
		else
		{
			console.log("Failed to parse data from socket as JSON. Data:");
			console.log(data);
		}
	};
	
	// Command Console
	self.sendCommand = function()
	{
		var command = self.commandText();
		self.commandText("");
		
		var commandData = command.split(" ");
		
		var index = self.commandHistory().length;
		self.commandPosition = index + 1;
		self.commandHistory.push(new Command({"command": command}));
		
		var allowedCommands = ["setJointGains", "setJointAngle", 
			"getJointAngle", "getJointLimits",
			"getJointGains", "getjointError"];
		
		if($.inArray(commandData[0], allowedCommands) != -1)
		{
			if(!isNaN(Number(commandData[1])))
			{
				commandData[1] = Number(commandData[1]);
			}

			var command = ko.toJSON(
				{
					command: commandData[0],
					jointNumber: commandData[1]
				});

			switch(commandData[0])
			{
				case "setJointGains":
					if(!isNaN(Number(commandData[2])))
					{
						command.PGain = Number(commandData[2]);
						
						if(!isNaN(Number(commandData[3])))
						{
							command.IGain = Number(commandData[3]);
						
							if(!isNaN(Number(commandData[4])))
							{
								command.DGain = Number(commandData[4]);
							
								self.socket.send(command);
							}
						}
					}

					break;

				case "setJointAngle":
					if(!isNaN(Number(commandData[2])))
					{
						command.angle = Number(commandData[2]);
						
						self.socket.send(command);
					}

					break;

				case "getJointAngle":
				case "getJointLimits":
				case "getJointGains":
				case "getjointError":
					self.socket.send(command);
					break;

				default:
					self.commandHistory()[index].response("Invalid command");
					break;
			}
			
			console.log("Data Sent: " + command);
		}
		else
		{
			self.commandHistory()[index].response("Invalid command");
		}
	};
	
	self.previousCommand = function()
	{
		if(self.commandPosition > 0)
		{
			self.commandPosition -= 1;
			self.commandText(self.commandHistory()[self.commandPosition].command());
		}
	};
	
	self.nextCommand = function()
	{
		if(self.commandPosition == self.commandHistory().length)
		{
			self.commandText("");
		}
		else if(self.commandPosition == self.commandHistory().length -1)
		{
			self.commandPosition += 1;
			self.commandText("");
		}
		else if(self.commandPosition < self.commandHistory().length - 1)
		{
			self.commandPosition += 1;
			self.commandText(self.commandHistory()[self.commandPosition].command());
		}
	};

	// Other
	self.addJoint = function(data)
	{
		self.joints.push(new Joint(data, self));
	};

	self.initSocket();
	self.init3D();
}

var vm = new PrometheusViewModel()

vm.addJoint({
	title: "Base",
	jointNumber: 0,
	min: 0,
	max: 360,
	setPoint: 0
});

vm.addJoint({
	title: "Shoulder",
	jointNumber: 1,
	min: 0,
	max: 180,
	setPoint: 0
});

vm.addJoint({
	title: "Elbow",
	jointNumber: 2,
	min: 180,
	max: 360,
	setPoint: 360
});

vm.addJoint({
	title: "Wrist",
	jointNumber: 3,
	min: 0,
	max: 359,
	setPoint: 180
});

vm.addJoint({
	title: "Hand",
	jointNumber: 4,
	min: 0,
	max: 359,
	setPoint: 180
});

ko.applyBindings(vm);
