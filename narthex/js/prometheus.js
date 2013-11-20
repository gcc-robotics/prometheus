function Joint(data)
{
	var self = this;
	
	self.title = data.title;
	self.jointNumber = data.jointNumber;
	
	self.min = data.min;
	self.max = data.max;
	self.setPoint = ko.observable(data.setPoint);
	
	self.setPoint.subscribe(function(newValue)
	{
		if(vm.enabled())
		{
			console.log("Joint " + self.jointNumber + " setPoint: " + newValue);
			
			$.ajax("/command", {
				data: ko.toJSON(
				{
					jointNumber: self.jointNumber, 
					setPoint: newValue
				}),
				type: "post", contentType: "application/json",
				success: function(result)
				{
					console.log(result);
				}
			});
		}
		else
		{
			console.log("Manual control disabled.")
		}
	});
}

function PrometheusViewModel()
{
	// Data
	var self = this;
	
	self.textEnabled = ko.observable("off");
	self.enabled = ko.computed(function()
	{
		return self.textEnabled() == "on" ? true : false;
	}, self);
	
	self.joints = ko.observableArray([]);

	// Operations
	self.addJoint = function(data)
	{
		self.joints.push(new Joint(data));
	};
}

var vm = new PrometheusViewModel()

vm.addJoint({
	title: "Base",
	jointNumber: 0,
	min: 0,
	max: 359,
	setPoint: 180
});

vm.addJoint({
	title: "Shoulder",
	jointNumber: 1,
	min: 0,
	max: 359,
	setPoint: 180
});

vm.addJoint({
	title: "Elbow",
	jointNumber: 2,
	min: 0,
	max: 359,
	setPoint: 180
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

