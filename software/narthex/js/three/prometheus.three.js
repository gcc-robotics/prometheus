function RobotArm(viewModel)
{
	var self = this;
	self.initialized = false;
	self.ready = false;
	self.animationFrameRequestId = 0;
	
	self.viewModel = viewModel;

	self.container = $('#3d-prometheus-container');
	self.width = 970;
	self.height = 500;

	self.demoIterators = 
	{
		0: -1,
		1: -1,
		2: -1,
		3: -1,
		4: -1
	};

	self.demoDirections = 
	{
		0: 1,
		1: 1,
		2: 1,
		3: 1,
		4: 1
	};
	
	self.cameraIterator = 0;
	self.cameraDistance = 750;

	self.geometriesToLoad = 0;
	self.geometriesLoaded = 0;
	
	// Methods
	self.init = function()
	{
		if(self.initialized === true)
		{
			self.requestAnimationFrame();
		}
		else
		{
			self.initialized = true;
			
			// Renderer
			self.renderer = new THREE.WebGLRenderer();
			self.renderer.setSize(self.width, self.height);

			self.container.get(0).appendChild(self.renderer.domElement);
			
			// Camera
			self.camera = new THREE.PerspectiveCamera(45, self.width / self.height, 0.1, 10000);
			self.camera.position.z = self.cameraDistance;
			self.camera.position.y = self.cameraDistance;
			self.camera.lookAt(new THREE.Vector3(0, 300, 0));
			
			// Enable auto-resizing of the renderer canvas
			self.startAutoResize();

			// Scene
			self.scene = new THREE.Scene();

			// Materials
			self.actualPositionArmMaterial =
				new THREE.MeshLambertMaterial(
				{
					color: 0xCC0000
				});

			self.setPointPositionArmMaterial =
				new THREE.MeshLambertMaterial(
				{
					color: 0xCCCCCC,
					transparent: true,
					opacity: 0.5
				});

			// First Light
			var light = new THREE.PointLight(0xFFFFFF);

			light.position.x = 0;
			light.position.z = self.cameraDistance;
			light.position.y = self.cameraDistance * 1.5;
			self.scene.add(light);
			
			// Second Light
			light = light.clone();
			light.position.z = -self.cameraDistance;
			self.scene.add(light);
			
			// Ground Plane
			self.ground = new THREE.Mesh(new THREE.PlaneGeometry(5000, 5000, 40, 40), new THREE.MeshBasicMaterial({ wireframe: true, color: 0x999999 }));
			self.ground.rotation.x = Math.PI / 2;
			self.scene.add(self.ground);

			// Arm Mesh Groups
			self.actualPositionArm = new THREE.Object3D();
			self.actualPositionArm.parts = {};
			self.setPointPositionArm = new THREE.Object3D();
			self.setPointPositionArm.parts = {};

			// Init STL Loader
			self.loader = new THREE.STLLoader();

			self.loader.addEventListener('load', function(event)
			{
				var geometry = event.content;

				var mesh = new THREE.Mesh(geometry, self.actualPositionArmMaterial);
				mesh.name = geometry.name;
				self.actualPositionArm.parts[mesh.name] = mesh;

				var mesh = new THREE.Mesh(geometry.clone(), self.setPointPositionArmMaterial);
				mesh.name = geometry.name;
				self.setPointPositionArm.parts[mesh.name] = mesh;

				self.geometriesLoaded += 1;
				
				if(self.geometriesLoaded == self.geometriesToLoad)
				{
					self.loadingComplete();
				}
			});

			// Load STL Files
			self.geometriesToLoad = 5;
			
			self.loader.load('./models/base.stl');
			self.loader.load('./models/torso.stl');
			self.loader.load('./models/humerus.stl');
			self.loader.load('./models/forearm-back.stl');
			self.loader.load('./models/forearm-front.stl');
		}
	};
	
	self.startAutoResize = function()
	{
		window.addEventListener('resize', self.autoResizeCallback, false);
		self.autoResizeCallback();
	};
	
	self.stopAutoResize = function()
	{
		window.removeEventListener('resize', self.autoResizeCallback);
	};
	
	self.autoResizeCallback = function()
	{
		self.renderer.setSize(self.container.width(), self.container.height());
		
		self.camera.aspect = self.container.width() / self.container.height();
		self.camera.updateProjectionMatrix();
	};
	
	self.addLine = function(parent, start, end)
	{
		// Line width doesn't work on windows
		var material = new THREE.LineBasicMaterial({linewidth: 3, color: 0x00FF00});
		
		var geometry = new THREE.Geometry;
		geometry.vertices.push(start);
		geometry.vertices.push(end);
		
		parent.add(new THREE.Line(geometry, material));
	};
	
	self.loadingComplete = function()
	{
		self.setupArmParts(self.actualPositionArm);
		self.scene.add(self.actualPositionArm);

		self.setupArmParts(self.setPointPositionArm, false);
		self.scene.add(self.setPointPositionArm);

		console.log('3D: Loading complete!');
		
		// Start the animation
		self.ready = true;
		self.requestAnimationFrame();
	};

	self.setupArmParts = function(arm, addAxisLines)
	{
		if(typeof(addAxisLines) === 'undefined') addAxisLines = true;


		// Add base to the arm
		arm.add(arm.parts['base']);
		
		// Move the axis of rotation for the base to it's center
		// Base Bounding box size: ~152.34, ~143.65, ~152.40
		arm.parts['base'].geometry.computeBoundingBox();
		var bb = arm.parts['base'].geometry.boundingBox;
		var offset = new THREE.Vector3();
		offset.addVectors(bb.min, bb.max);
		offset.multiplyScalar(-0.5);
		arm.parts['base'].geometry.applyMatrix(new THREE.Matrix4().makeTranslation(offset.x, 0, offset.z));
		arm.parts['base'].geometry.computeBoundingBox();
		
		// Add torso to base
		arm.parts['base'].add(arm.parts['torso']);
		
		// Move the axis of rotation for the torso to the center of it's base
		// Torso Bounding box size: ~152.36, ~325.48, ~152.40
		arm.parts['torso'].geometry.computeBoundingBox();
		var bb = arm.parts['torso'].geometry.boundingBox;
		var offset = new THREE.Vector3();
		offset.addVectors(bb.min, bb.max);
		offset.multiplyScalar(-0.5);
		arm.parts['torso'].geometry.applyMatrix(new THREE.Matrix4().makeTranslation(offset.x, 144, offset.z));
		arm.parts['torso'].geometry.computeBoundingBox();
		
		// Add humerus to torso
		arm.parts['torso'].add(arm.parts['humerus']);
		
		// Move the axis of rotation for the humerus
		// Humerus Bounding Box Size: ~114.30, ~352.95, ~217.95
		arm.parts['humerus'].geometry.applyMatrix(new THREE.Matrix4().makeTranslation(-57.15, -328, -193));
		arm.parts['humerus'].geometry.computeBoundingBox();
		
		// Move humerus to the correct position
		// The axis for jointing the torso to the humerus is 425 up from the center of base
		arm.parts['humerus'].position.y = 425;
		
		// Add the back of the forearm to the humerus
		arm.parts['humerus'].add(arm.parts['forearm-back']);
		
		// Move the axis of rotation for forearm-back
		// forearm-back Bounding Box Size: ~88.90, ~88.90, ~215.12
		arm.parts['forearm-back'].geometry.computeBoundingBox();
		var bb = arm.parts['forearm-back'].geometry.boundingBox;
		var offset = new THREE.Vector3();
		offset.addVectors(bb.min, bb.max);
		offset.multiplyScalar(-0.5);
		arm.parts['forearm-back'].geometry.applyMatrix(new THREE.Matrix4().makeTranslation(offset.x, offset.y, -195));
		arm.parts['forearm-back'].geometry.computeBoundingBox();
		
		// Move forearm-back to the correct position
		arm.parts['forearm-back'].position.y = -290;
		arm.parts['forearm-back'].position.z = -136;
		
		// Add the front of the forearm to the forearm-back
		arm.parts['forearm-back'].add(arm.parts['forearm-front']);
		
		// Move the axis of rotation for forearm-front
		THREE.GeometryUtils.center(arm.parts['forearm-front'].geometry);
		
		// Move forearm-front to the correct position
		arm.parts['forearm-front'].position.z = -266.5; // -265

		if(addAxisLines === true)
		{
			// Add lines for axes of rotation
			self.addLine(self.scene, new THREE.Vector3(0, 69, 0), new THREE.Vector3(0, 219, 0));
			self.addLine(arm.parts['torso'], new THREE.Vector3(100, 425, 0), new THREE.Vector3(-100, 425, 0));
			self.addLine(arm.parts['humerus'], new THREE.Vector3(100, -290, -135), new THREE.Vector3(-100, -290, -138));
			self.addLine(arm.parts['forearm-back'], new THREE.Vector3(0, 0, -238), new THREE.Vector3(0, 0, -138));
		}
	};
	
	self.animate = function()
	{
		if(self.viewModel.Render3DEnabled() && self.ready === true)
		{
			self.requestAnimationFrame();
		
			// Move actualPositionArm meshes according to the current joint angles
			self.actualPositionArm.parts['torso'].rotation.y = self.viewModel.joints()[0].currentAngle() / 180 * Math.PI;
			self.actualPositionArm.parts['humerus'].rotation.x = self.viewModel.joints()[1].currentAngle() / 180 * Math.PI;
			self.actualPositionArm.parts['forearm-back'].rotation.x = self.viewModel.joints()[2].currentAngle() / 180 * Math.PI;
			self.actualPositionArm.parts['forearm-front'].rotation.z = self.viewModel.joints()[3].currentAngle() / 180 * Math.PI;

			// Move setPointPositionArm meshes according to the joint set points
			self.setPointPositionArm.parts['torso'].rotation.y = self.viewModel.joints()[0].setPoint() / 180 * Math.PI;
			self.setPointPositionArm.parts['humerus'].rotation.x = self.viewModel.joints()[1].setPoint() / 180 * Math.PI;
			self.setPointPositionArm.parts['forearm-back'].rotation.x = self.viewModel.joints()[2].setPoint() / 180 * Math.PI;
			self.setPointPositionArm.parts['forearm-front'].rotation.z = self.viewModel.joints()[3].setPoint() / 180 * Math.PI;
			
			if(self.viewModel.Rotate3DEnabled())
			{
				// Rotate camera around 0,300,0
				self.cameraIterator += 0.3;
				self.camera.position.x = Math.sin(self.cameraIterator / 180 * Math.PI) * self.cameraDistance;
				self.camera.position.z = Math.cos(self.cameraIterator / 180 * Math.PI) * self.cameraDistance;
				self.camera.lookAt(new THREE.Vector3(0, 300, 0)); // 0, 300, 0
			}

			// Run the demo if it's enabled
			if(self.viewModel.armDemoEnabled())
			{
				for(var i = self.viewModel.joints().length - 1; i >= 0; i--)
				{
					var joint = self.viewModel.joints()[i];

					var range = joint.max() - joint.min();
					var value = joint.min() + (self.demoIterators[i] % range);
					self.viewModel.joints()[i].setPoint(value);
				
					self.demoIterators[i] += self.demoDirections[i] * 0.1;

					if(self.demoIterators[i] > joint.max())
					{
						self.demoIterators[i] = joint.max();
						self.demoDirections[i] *= -1;
					}

					if(self.demoIterators[i] < joint.min())
					{
						self.demoIterators[i] = joint.min();
						self.demoDirections[i] *= -1;
					}
				};
			}
			else
			{
				for(var i = self.demoIterators.length - 1; i >= 0; i--)
				{
					self.demoIterators[i] = -1;
				};
			}
			
			// Render the scene
			self.renderer.render(self.scene, self.camera);
			self.animationFrameRequestId = 0;
		}
	};
	
	self.requestAnimationFrame = function()
	{
		if(self.animationFrameRequestId != 0)
		{
			window.cancelAnimationFrame(self.animationFrameRequestId);
		}
		
		self.animationFrameRequestId = window.requestAnimationFrame(self.animate);
	};
}
