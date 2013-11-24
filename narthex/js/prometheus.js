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

function Joint(data)
{
	var self = this;
	
	self.title = data.title;
	self.jointNumber = ko.observable(data.jointNumber);
	self.inputId = ko.computed(function()
	{
		return self.title.toLowerCase() + "-angle";
	});
	
	self.min = ko.observable(data.min);
	self.max = ko.observable(data.max);
	self.setPoint = ko.observable(data.setPoint).extend({ notify: 'always' });;
	
	// An unnamed computed observable to trigger AJAX.
	// Throttled by 100ms to prevent multiple AJAX requests 
	// being sent as the user changes the slider
	//
	// TODO: Maybe instead of throttling a computed observable
	// we should use self.setPoint.subscribe and issue the AJAX
	// request immediately and then cancel it if it hasn't finished
	// by the time the function is called again? This would prevent 
	// AJAX requests from being sent when vm.enabled() and 
	// self.jointNumber() change like they do now and remove the 
	// 100ms of latency introduced by the throttling.
	ko.computed(function()
	{		
		if(vm.enabled())
		{
			console.log("Joint " + self.jointNumber() + " setPoint: " + self.setPoint());
			
			$.ajax("/command/setJointAngle/", {
				data: ko.toJSON(
				{
					jointNumber: self.jointNumber(), 
					setPoint: self.setPoint()
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
	}).extend({throttle: 100});
}

function PrometheusViewModel()
{
	// Data
	var self = this;
	
	self.textEnabled = ko.observable("off");
	self.enabled = ko.computed(function()
	{
		return self.textEnabled() == "on" ? true : false;
	});
	
	self.joints = ko.observableArray([]);

	// Operations
	self.addJoint = function(data)
	{
		self.joints.push(new Joint(data));
	};
}

var vm = new PrometheusViewModel()

var arm = new RobotArm(vm);
arm.init();

vm.addJoint({
	title: "Base",
	jointNumber: 0,
	min: 0,
	max: 359,
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
